#include <thread>
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
	std::cout << "network" << std::endl;
	while (isInstance_)
	{

		// Guest
		IpNetWorkState->RevUpdate();
		auto data = IpNetWorkState->GetRevPacket();
		if (log.is_open() == true)
		{
			for (auto DATA : data)
			{
				std::cout << DATA << std::endl;
				log << DATA << "\n";
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
	
	std::ifstream start_time("Login.txt");
	std::vector<int> timeList;
	std::string line;
	Header headerdata{MesType::NON};

	while (!start_time.eof())
	{
		std::getline(start_time, line);
		timeList.emplace_back(std::stoi(line));
	}

	int idx = 0;
	while (idx < timeList.size())
	{
		if (static_cast<MesType>(timeList[idx]) == MesType::COUNT_DOWN_GAME)
		{
			headerdata.data_[0] = timeList[idx];
			headerdata.data_[1]= timeList[idx++];
			if (headerdata.mesdata_.length_ == 2)
			{
				headerdata.data_[0]= timeList[idx++];
				headerdata.data_[1]= timeList[idx++];
				break;
			}
		}
		idx++;
	}
	std::cout<<std::chrono::time_point(headerdata.start)
	std::thread netWorkThread(&GameScene::Network, this);
	netWorkThread.detach();
	log.open("Game.txt", std::ios::beg | std::ios::out);

	std::cout << "-------------初期化終了---------------" << std::endl;
	std::cin.get();
	flame = 0;
	return false;
}
