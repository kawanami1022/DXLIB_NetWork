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
	IpNetWork->GetNetWorkState()->ClearDataPacket();
	unsigned short id = 0;
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
		int flag = (id == character_.size() - 1) ? 0 : 1;
		Header headerData{ MesType::POS,flag,id,static_cast<int>(CHAR->GetMoveDir()) };
		IpNetWork->GetNetWorkState()->SetDataPacket(headerData.data_[0]);
		IpNetWork->GetNetWorkState()->SetDataPacket(headerData.data_[1]);
		auto charPos = CHAR->GetPos();
		IpNetWork->GetNetWorkState()->SetDataPacket(charPos.x);
		IpNetWork->GetNetWorkState()->SetDataPacket(charPos.y);
		id++;
	}
	IpNetWork->GetNetWorkState()->RevUpdate();

}

void GameScene::UpdateGuest()
{
	IpNetWork->GetNetWorkState()->RevUpdate();
	auto dataPacket = IpNetWork->GetNetWorkState()->GetDataPacket();

	for (auto DATAPACKET : dataPacket)
	{
		std::cout << "受け取ったデータ:";
		std::cout << DATAPACKET << std::endl;
	}
}

void GameScene::UpdateOFFLINE()
{
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
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

	// sponePlayer
	VecCharacter sponePlayer= map_->SponePlayer();
	character_.insert(character_.end(), sponePlayer.begin(), sponePlayer.end());
	return false;
}
