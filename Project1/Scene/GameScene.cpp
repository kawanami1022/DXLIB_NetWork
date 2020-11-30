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
	std::thread netWorkThread(&GameScene::Network,this);
	netWorkThread.join();
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}
	
	IpNetWorkState->SendUpdate();
}

void GameScene::UpdateGuest()
{
	std::thread netWorkThread(&GameScene::Network, this);
	netWorkThread.join();
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}
	IpNetWorkState->SendUpdate();
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
	
	IpNetWorkState->RevUpdate();
	auto revPacket = IpNetWorkState->GetRevPacket();
	//std::cout << "取得したデータサイズ:" << revPacket.size() << std::endl;

	while (revPacket.size() >=5)
	{
		try
		{
			if (revPacket.at(0) == static_cast<int>(MesType::POS))
			{
				for (auto PLAYER : character_)
				{

					int playerPos[] = { revPacket.at(0),revPacket.at(1),revPacket.at(2),revPacket.at(3),revPacket.at(4) };
					if (PLAYER->GetPlID() == playerPos[1])
					{
						PLAYER->SetPos(Position2(playerPos[2], playerPos[3]));
						PLAYER->SetDir(static_cast<MoveDir>(playerPos[4]));
						revPacket.erase(revPacket.begin(), revPacket.begin() + 5);
					}
				}
			}
		}
		catch (...)
		{
			std::cout << "illegal access" << std::endl;
		}
	}
}


GameScene::GameScene()
{
	std::cout << "-------------GameScene---------------" << std::endl;
	Init();
}

GameScene::~GameScene()
{
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
	for (unsigned int y = 0; y < map_->GetMapSize().y+1; y++)
	{
		for (unsigned int x = 0; x < map_->GetMapSize().x+1; x++)
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

	std::cout << "-------------初期化終了---------------" << std::endl;

	flame = 0;
	return false;
}
