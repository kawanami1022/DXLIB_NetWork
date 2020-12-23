#include <thread>

#include <filesystem>
#include <iostream>
#include <DxLib.h>
#include "../Lib/File/TMX_File.h"
#include "../NetWork/NetWork.h"
#include "../NetWork/NetWorkState.h"
#include "../Actor/map/map.h"
#include "../Actor/Character/Character.h"
#include "../Actor/Item/Item.h"
#include "GameScene.h"

UniqueBase GameScene::input(UniqueBase nowScene)
{
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
		std::cout << "開始時間:" << std::chrono::duration_cast<std::chrono::milliseconds>(headerdata.start_.time_since_epoch()).count() << std::endl;
		std::cout << "現在時間:" << std::chrono::duration_cast<std::chrono::milliseconds>(clock.now().time_since_epoch()).count() << std::endl;
		
	};

	std::call_once(once, InitStartTime);
	auto netWorkMode = IpNetWork->GetNetWorkMode();
	updateNetWorkModeFunc_[netWorkMode]();
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
			//炎エフェクトが存在するかたしかめる
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

	}
	

	// 削除する処理
	for (auto idx = 0; idx < bomb_.size(); idx++)
	{
		if (bomb_[idx]->GetBombState() == BOMB_STATE::DETH)
		{
			bomb_.erase(bomb_.begin() + idx);
		}
	}

	for (auto idx = 0; idx < fire_.size(); idx++)
	{
		if (fire_[idx]->GetState() == EXP_STATE::DEAD)
		{
			fire_.erase(fire_.begin() + idx);
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
	//Guest 側の処理
	Header headerdata{ MesType::NON };

	if (IpNetWorkState->GetNetWorkMode() == NetWorkMode::GUEST)
	{
		std::cout << "network" << std::endl;
		while (isInstance_)
		{
			// Guest
			IpNetWorkState->RevUpdate();
			auto data = IpNetWorkState->GetRevPacket();
			while (data.size() > 0)
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
							// playerを殺す
							CHARACTER_->SetState(CharState::Death);
							data.erase(data.begin());
							break;
						}
					}
				}
				if (headerdata.mesdata_.type == MesType::SET_BOM)
				{
					bomb_.emplace_back(std::make_unique<Bomb>(Position2(data[2], data[3])));
					// long long型 爆弾設置時間
					Header start = { MesType::SET_BOM };
					start.data_[0] = data[5];
					start.data_[1] = data[6];
					bomb_.back()->SetSetTime(start.start_);
					// 爆発時間
					bomb_.back()->SetExTime(data[4]);

					data.erase(data.begin(), data.begin() + headerdata.data_[1]);
				}

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

GameScene::GameScene()
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
	std::cout << "-------------初期化開始---------------" << std::endl;
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
					std::cout << "例外" << std::endl;
					std::cout << e.what() << std::endl;
				}
			}
		}

	}

	std::thread netWorkThread(&GameScene::Network, this);
	netWorkThread.detach();
	std::cout << "-------------初期化終了---------------" << std::endl;
	std::cin.get();
	flame = 0;
	return false;
}
