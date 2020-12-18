#include <thread>

#include <filesystem>
#include <iostream>
#include <DxLib.h>
#include "../Lib/File/TMX_File.h"
#include "../NetWork/NetWork.h"
#include "../NetWork/NetWorkState.h"
#include "../Actor/map/map.h"
#include "../Actor/Character/Character.h"
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
	bomb_.Draw();
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

	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
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
					data.erase(data.begin(), data.begin() + headerdata.data_[1]);
				}

			}
			
			// playerの送信する

			// playerのIDを取得
			auto id = IpNetWorkState->GetPlID();
		
			headerdata.mesdata_ = { MesType::POS,0,0,4 };		// ヘッダーデータを送信
			IpNetWorkState->SetSendPacket(headerdata.data_[0]);		// MesType::POS,0,0
			IpNetWorkState->SetSendPacket(headerdata.data_[1]);		// 4
			IpNetWorkState->SetSendPacket(id);									// GuestのplayerID送信					
			for (auto CHARACTER_ : character_)
			{
				// playerIDのplayerを探す
				if (id == CHARACTER_->GetPlID())
				{
					
					auto pos = CHARACTER_->GetPos();				// 座標保存用変数
					auto dir = CHARACTER_->GetMoveDir();		// 方向保存用変数
					IpNetWorkState->SetSendPacket(pos.x);			// 座標x:送信
					IpNetWorkState->SetSendPacket(pos.y);			// 座標y:送信
					IpNetWorkState->SetSendPacket(static_cast<int>(dir));		// 方向:送信
				}
			}


		}
		


	}

#ifdef DEBUG
	while (isInstance_)
	{

	}

#endif // !1
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
	for (unsigned int y = 0; y < map_->GetMapSize().y; y++)
	{
		for (unsigned int x = 0; x < map_->GetMapSize().x; x++)
		{
			Position2 TilePos = Position2(x, y);
			if (map_->GetGridID(TilePos, "character")==4)
			{
				try
				{
					character_.emplace_back(std::make_unique<Character>(TilePos * TileSize));
				}
				catch (std::out_of_range& e)
				{
					std::cout << "例外" << std::endl;
					std::cout << e.what() << std::endl;
				}
			}
		}
	}

	bomb_ = Bomb();

	std::thread netWorkThread(&GameScene::Network, this);
	netWorkThread.detach();
	std::cout << "-------------初期化終了---------------" << std::endl;
	std::cin.get();
	flame = 0;
	return false;
}
