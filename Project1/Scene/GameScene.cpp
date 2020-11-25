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
}

void GameScene::UpdateHost()
{
	Network();
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
	}
	IpNetWorkState->SendUpdate();
}

void GameScene::UpdateGuest()
{
	Network();
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
	
	//IpNetWorkState->RevUpdate();
	//auto revPacket = IpNetWorkState->GetRevPacket();
	//std::cout << "取得したデータサイズ:" << revPacket.size() << std::endl;

	//if (revPacket[0] == static_cast<int>(MesType::POS))
	//{
	//	try
	//	{
	//		int playerPos[] = { revPacket[0],revPacket[1],revPacket[2] ,revPacket[3],revPacket[4] };
	//		character_[revPacket[4]]->SetPos(Position2(revPacket[2], revPacket[3]));
	//		character_[revPacket[4]]->SetDir(static_cast<MoveDir>(revPacket[4]));
	//		revPacket.erase(revPacket.begin(), revPacket.begin() + 5);
	//	}
	//	catch (...)
	//	{
	//		std::cout << "illegal access" << std::endl;
	//	}
	//}
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
	std::cout << "-------------初期化終了---------------" << std::endl;
	return false;
}
