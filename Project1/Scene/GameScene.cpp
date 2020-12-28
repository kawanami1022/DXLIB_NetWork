#include <thread>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <DxLib.h>
#include "../Lib/File/TMX_File.h"
#include "../NetWork/NetWork.h"
#include "../NetWork/NetWorkState.h"
#include "../Actor/map/map.h"
#include "../Actor/Character/Character.h"
#include "../Actor/Item/Item.h"
#include "ResultScene.h"
#include "CrossOver.h"
#include "GameScene.h"

UniqueBase GameScene::input(UniqueBase nowScene)
{
	// ���e�ݒu
	Header header{ MesType::NON };
	for (auto& CHARACTER : character_)
	{
		if (CHARACTER->GetSetBb())
		{
			bomb_.emplace_back(std::make_unique<Bomb>(CHARACTER->GetPos()));
			if (IpNetWorkState->GetNetWorkMode() == NetWorkMode::GUEST)
			{
				header.mesdata_ = { MesType::SET_BOM ,0,0,7 };
				IpNetWorkState->SetSendPacket(header.data_[0]);
				IpNetWorkState->SetSendPacket(7);
				IpNetWorkState->SetSendPacket(CHARACTER->GetPlID());
				IpNetWorkState->SetSendPacket(bomb_.size());
				IpNetWorkState->SetSendPacket(CHARACTER->GetPos().x);
				IpNetWorkState->SetSendPacket(CHARACTER->GetPos().y);
				IpNetWorkState->SetSendPacket(3);
				header.start_ = bomb_.back()->GetSetTime();
				IpNetWorkState->SetSendPacket(header.data_[0]);
				IpNetWorkState->SetSendPacket(header.data_[1]);
				std::cout <<"bomb��selfID:"<< CHARACTER->GetPlID() + bomb_.size() << std::endl;
			}
		}
	}
	return nowScene;

}

UniqueBase GameScene::UpDate(UniqueBase nowScene)
{
	auto InitStartTime = [&]() {

		std::ifstream start_time("Login.txt");
		std::vector<int> timeList;
		std::string line;
		Header headerdata{ MesType::NON };
		start_time.seekg(std::ios::beg);

		while (!start_time.eof())
		{
			std::getline(start_time, line);
			if (line.size() <= 0)continue;
			timeList.emplace_back(std::stoi(line));
			std::cout << line << std::endl;
		}

		int idx = 0;
		while (idx < timeList.size())
		{
			if (static_cast<MesType>(timeList[idx]) == MesType::COUNT_DOWN_GAME)
			{
				headerdata.data_[0] = timeList[idx];
				headerdata.data_[1] = timeList[idx + 1];
				if (headerdata.mesdata_.length_ == 2)
				{
					headerdata.data_[0] = timeList[idx + 2];
					headerdata.data_[1] = timeList[idx + 3];
					break;
				}
			}
			idx++;
		}
		std::chrono::system_clock clock;
		std::cout << "�J�n����:" << std::chrono::duration_cast<std::chrono::milliseconds>(headerdata.start_.time_since_epoch()).count() << std::endl;
		std::cout << "���ݎ���:" << std::chrono::duration_cast<std::chrono::milliseconds>(clock.now().time_since_epoch()).count() << std::endl;
		
	};

	std::call_once(once, InitStartTime);
	auto netWorkMode = IpNetWork->GetNetWorkMode();
	// �V�[���@��
	updateNetWorkModeFunc_[netWorkMode]();
	if (changeScene_)
	{
		auto nextScene = std::make_unique<ResultScene>();
		nowScene = std::make_unique<CrossOver>(std::move(nowScene), std::move(nextScene));
	}
	SetDrawScreen(screenSrcID_);
	ClsDrawScreen();
	Draw();
	flame++;
	return nowScene;
}

void GameScene::Draw()
{
	SetDrawScreen(screenSrcID_);
	map_->Draw();
	for (auto CHARACTER : character_)
	{
		CHARACTER->Draw();
	}
	for (auto &BOMB : bomb_)
	{
		BOMB->Draw();
	}
	for (auto& FIRE : fire_)
	{
		FIRE->Draw();
	}
}

void GameScene::UpdateHost()
{

	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}

}

void GameScene::UpdateGuest()
{
	auto IsFire = [&](std::unique_ptr<Bomb>& bomb) {
		if (bomb->GetBombState() == BOMB_STATE::IGNITED)
		{
			//���G�t�F�N�g�����݂��邩�������߂�
			for (auto& FIRE : fire_)
			{
				if (FIRE->GetPos() == bomb->GetPos())
				{

					return false;
				}
			}
			return	true;
		}else {return false;}
	};

	// 2�ȏ�̉������݂��Ȃ����������߂�
	auto CheckMoreSecFire = [&](Position2 pos,int dst) {
		for (auto& FIRE : fire_)
		{
			if (FIRE->GetPos() == pos)
			{
				return false;
			}
		}
		for (auto& FIRE : fire_)
		{
			if (FIRE->GetPos() != pos)
			{
				std::cout << FIRE->GetPos().x << "," << FIRE->GetPos().y << ":" << pos.x << "," << pos.y << std::endl;
				if (FIRE->GetDst() != dst)
				{
					if (map_->GetMapId(pos) != MAP_ID::BLOCK_INBREAK)
					{
						return true;
					}
				}
			}
		}
		return false;
	};

	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}

	for (auto& BOMB : bomb_)
	{
		BOMB->Update();
		if (IsFire(BOMB))
		{
			SetFire(BOMB->GetPos(), 0);
		}

	}

	for (auto& FIRE : fire_)
	{

		FIRE->Update();

		if (FIRE->GetDst() == 0)
		{

			auto TilePos = FIRE->GetPos() / TileSize;

			if (FIRE->GetElTime() > std::chrono::milliseconds(1000 / 7 * 1))
			{

				if (CheckMoreSecFire(Position2(TilePos.x, TilePos.y - 1) * TileSize, 1) == true)
				{
					SetFire(Position2(TilePos.x, TilePos.y - 1) * TileSize, 1);
					map_->setMapId(Position2(TilePos.x, TilePos.y - 1), 0);
				}
				if (CheckMoreSecFire(Position2(TilePos.x + 1, TilePos.y) * TileSize, 1) == true)
				{
					SetFire(Position2(TilePos.x + 1, TilePos.y) * TileSize, 1);
					map_->setMapId(Position2(TilePos.x + 1, TilePos.y), 0);
				}
				if (CheckMoreSecFire(Position2(TilePos.x, TilePos.y + 1) * TileSize, 1) == true)
				{
					SetFire(Position2(TilePos.x, TilePos.y + 1) * TileSize, 1);
					map_->setMapId(Position2(TilePos.x, TilePos.y + 1), 0);
				}
				if (CheckMoreSecFire(Position2(TilePos.x - 1, TilePos.y) * TileSize, 1) == true)
				{
					SetFire(Position2(TilePos.x - 1, TilePos.y) * TileSize, 1);
					map_->setMapId(Position2(TilePos.x - 1, TilePos.y), 0);
				}
			}
#ifdef DEBUG


			if (FIRE->GetElTime() > std::chrono::milliseconds(1000 / 7 * 2))
			{
				if (map_->GetMapId(Position2(TilePos.x, TilePos.y - 2)) == MAP_ID::BLOCK_BREAK|| map_->GetMapId(Position2(TilePos.x, TilePos.y - 2)) == MAP_ID::BLOCK_INBREAK)
				{

					if (CheckMoreSecFire(Position2(TilePos.x, TilePos.y - 2) * TileSize, 2) == true)
					{
						SetFire(Position2(TilePos.x, TilePos.y - 2) * TileSize, 2);
					}
				}
				if (map_->GetMapId(Position2(TilePos.x + 2, TilePos.y)) == MAP_ID::BLOCK_BREAK || map_->GetMapId(Position2(TilePos.x + 2, TilePos.y)) == MAP_ID::BLOCK_INBREAK)
				{
					if (CheckMoreSecFire(Position2(TilePos.x + 2, TilePos.y) * TileSize, 2) == true)
					{
						SetFire(Position2(TilePos.x + 2, TilePos.y) * TileSize, 2);
					}
				}
				if (map_->GetMapId(Position2(TilePos.x, TilePos.y + 2)) == MAP_ID::BLOCK_BREAK || map_->GetMapId(Position2(TilePos.x, TilePos.y + 2)) == MAP_ID::BLOCK_INBREAK)
				{
					if (CheckMoreSecFire(Position2(TilePos.x, TilePos.y + 2) * TileSize, 2) == true)
					{
						SetFire(Position2(TilePos.x, TilePos.y + 2) * TileSize, 2);

					}
				}
				if (map_->GetMapId(Position2(TilePos.x - 2, TilePos.y)) == MAP_ID::BLOCK_BREAK || map_->GetMapId(Position2(TilePos.x - 2, TilePos.y)) == MAP_ID::BLOCK_INBREAK)
				{
					if (CheckMoreSecFire(Position2(TilePos.x - 2, TilePos.y) * TileSize, 2) == true)
					{
						SetFire(Position2(TilePos.x - 2, TilePos.y) * TileSize, 2);
					}
				}
			}
#endif // DEBUG

		}
		
	}


	// �폜���鏈��
	auto bomb = std::remove_if(bomb_.begin(), bomb_.end(), [&](std::unique_ptr<Bomb>& bomb) {
		return (bomb->GetBombState() == BOMB_STATE::DETH);
	});
	bomb_.erase(bomb, bomb_.end());
	bomb_.shrink_to_fit();


	for (auto idx = 0; idx < fire_.size(); idx++)
	{
		if (fire_[idx]->GetState() == EXP_STATE::DEAD)
	
		{
			fire_.erase(fire_.begin() + idx);
		}
	}
	for (auto& FIRE : fire_)
	{
		for (auto idx=0;idx<character_.size();idx++)
		{
			Header header{ MesType::DETH,0,0,1 };
			if (character_[idx]->GetPos() / TileSize == FIRE->GetPos() / TileSize|| character_[idx]->GetDRPos() / TileSize == FIRE->GetPos() / TileSize)
			{
				IpNetWorkState->SetSendPacket(header.data_[0]);
				IpNetWorkState->SetSendPacket(header.data_[1]);
				IpNetWorkState->SetSendPacket(character_[idx]->GetPlID());
				character_.erase(character_.begin() + idx);
			}
		}

	}

}

void GameScene::UpdateOFFLINE()
{
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}
}

void GameScene::Network()
{
	//Guest ���̏���
	Header headerdata{ MesType::NON };

	if (IpNetWorkState->GetNetWorkMode() == NetWorkMode::GUEST)
	{
		std::cout << "network" << std::endl;
		while (isInstance_)
		{
			// Guest
			IpNetWorkState->RevUpdate();
			auto data = IpNetWorkState->GetRevPacket();
			while (data.size() > 2)
			{
				headerdata.data_[0] = data[0];
				headerdata.data_[1] = data[1];
				data.erase(data.begin());
				data.erase(data.begin());
				if (headerdata.mesdata_.type == MesType::POS)
				{
					
					auto id = data.front();
					data.erase(data.begin());
					for (auto& CHARACTER_ : character_)
					{
						if (CHARACTER_->GetPlID() == id)
						{
							
							CHARACTER_->SetPos(Position2(data[0], data[1]));
							CHARACTER_->SetDir(static_cast<MoveDir>(data[2]));
							data.erase(data.begin(), data.begin() + 3);
							break;
						}
					}
				}
				if (headerdata.mesdata_.type == MesType::DETH)
				{
					for (auto& CHARACTER_ : character_)
					{
						auto id = data.front();
						if (CHARACTER_->GetPlID() == id)
						{
							// player���E��
							CHARACTER_->SetState(CharState::Death);
							data.erase(data.begin()+id/5);
							break;
						}
					}
				}
				if (headerdata.mesdata_.type == MesType::SET_BOM)
				{
					bomb_.emplace_back(std::make_unique<Bomb>(Position2(data[2], data[3])));
					// long long�^ ���e�ݒu����
					Header start = { MesType::SET_BOM };
					start.data_[0] = data[5];
					start.data_[1] = data[6];
					bomb_.back()->SetSetTime(start.start_);
					// ��������
					bomb_.back()->SetExTime(data[4]);
					std::ofstream bomb("Bomb.txt", std::ios::out|std::ios::end);
					bomb.seekp(std::ios::end);
					bomb << std::endl;
					bomb << data[0] << std::endl;
					bomb << data[1] << std::endl;
					bomb << data[2] << std::endl;
					bomb << data[3] << std::endl;
					bomb << data[4] << std::endl;
					bomb << data[5] << std::endl;
					bomb << data[6] << std::endl;
					std::cout << data[0] << std::endl;
					std::cout << data[1] << std::endl;
					std::cout << data[2] << std::endl;
					std::cout << data[3] << std::endl;
					std::cout << data[4] << std::endl;
					std::cout << data[5] << std::endl;
					std::cout << data[6] << std::endl;
					data.erase(data.begin(), data.begin() + headerdata.data_[1]);
				}

				if (headerdata.mesdata_.type == MesType::RESULT)
				{
					IpNetWorkState->SetResult(data[0]);
					IpNetWorkState->SetResult(data[1]);
					IpNetWorkState->SetResult(data[2]);
					IpNetWorkState->SetResult(data[3]);
					IpNetWorkState->SetResult(data[4]);
					changeScene_ = true;
				}

			}
			if (IpNetWorkState->GetSendPacket().size() > 0)
			{
				IpNetWorkState->SendUpdate(IpNetWorkState->GetNetWorkHandle().front());
			}
		}
	}
}


bool GameScene::SetBomb(int ownerID, int selfID, Vector2 pos, bool sendNet)
{
	return false;
}

bool GameScene::SetFire(Position2 pos, int dst)
{
	fire_.emplace_back(std::make_unique<explosion>(pos,dst));

	return true;
}

GameScene::GameScene():changeScene_(false)
{
	std::cout << "-------------GameScene---------------" << std::endl;
	Init();
}

GameScene::~GameScene()
{
	isInstance_ = false;
}

bool GameScene::Init()
{
	std::cout << "-------------�������J�n---------------" << std::endl;
	map_=std::make_shared<Map>();
	updateNetWorkModeFunc_ = 
	{ { NetWorkMode::OFFLINE,std::bind(&GameScene::UpdateOFFLINE,this) },
	{ NetWorkMode::HOST,std::bind(&GameScene::UpdateHost,this) },
	{ NetWorkMode::GUEST,std::bind(&GameScene::UpdateGuest,this) } };
	std::cout << "-------------MapClass---------------" << std::endl;
	IpNetWorkState->ClearRevPacket();
	IpNetWorkState->ClearSendPacket();
	std::cout << "-------------ClearPacket---------------" << std::endl;
	auto playerMax= IpNetWorkState->GetPIMax();
	auto tmpCharCnt = 0;
	for (unsigned int y = 0; y < map_->GetMapSize().y; y++)
	{
		if (tmpCharCnt >= playerMax){break;}
		for (unsigned int x = 0; x < map_->GetMapSize().x; x++)
		{
			if (tmpCharCnt >= playerMax){break;}

			Position2 TilePos = Position2(x, y);
			if (map_->GetGridID(TilePos, "character")==4)
			{
				try
				{
					character_.emplace_back(std::make_unique<Character>(TilePos * TileSize));
					tmpCharCnt++;
				}
				catch (std::out_of_range& e)
				{
					std::cout << "��O" << std::endl;
					std::cout << e.what() << std::endl;
				}
			}
		}

	}

	std::thread netWorkThread(&GameScene::Network, this);
	netWorkThread.detach();
	std::cout << "-------------�������I��---------------" << std::endl;
	std::cin.get();
	flame = 0;
	return false;
}
