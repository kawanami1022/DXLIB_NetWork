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
	
	IpNetWorkState->RevUpdate();
	auto revPacket = IpNetWorkState->GetRevPacket();
	std::cout << "取得したデータサイズ:" << revPacket.size() << std::endl;
	while (revPacket.size() > 0)
	{
		if (revPacket[0] == static_cast<int>(MesType::POS))
		{
			try
			{
				int playerPos[] = { revPacket[0],revPacket[1],revPacket[2] ,revPacket[3],revPacket[4] };
				character_[revPacket[4]]->SetPos(Position2(revPacket[2], revPacket[3]));
				character_[revPacket[4]]->SetDir(static_cast<MoveDir>(revPacket[4]));
				revPacket.erase(revPacket.begin(), revPacket.begin() + 5);
			}
			catch (...)
			{
				std::cout << "illegal access" << std::endl;
			}
		}
	}
}


GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

bool GameScene::Init()
{
	map_=std::make_shared<Map>();
	updateNetWorkModeFunc_ = 
	{ { NetWorkMode::OFFLINE,std::bind(&GameScene::UpdateOFFLINE,this) },
	{ NetWorkMode::HOST,std::bind(&GameScene::UpdateHost,this) },
	{ NetWorkMode::GUEST,std::bind(&GameScene::UpdateGuest,this) } };

	IpNetWorkState->ClearRevPacket();
	IpNetWorkState->ClearSendPacket();
	// sponePlayer
	VecCharacter sponePlayer= map_->SponePlayer();
	character_.insert(character_.end(), sponePlayer.begin(), sponePlayer.end());
	return false;
}
