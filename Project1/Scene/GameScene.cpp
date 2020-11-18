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
	unsigned short id = 0;
	for (auto CHAR : character_)
	{
		CHAR->Update(map_);
		int flag = (id == character_.size())? 0 : 1;
		Header headerData{ MesType::POS,flag,id,0 };
		IpNetWork->GetNetWorkState()->SetMesPacket(headerData.data_[0]);
		auto charPos = CHAR->GetPos();
		IpNetWork->GetNetWorkState()->SetMesPacket(charPos.x);
		IpNetWork->GetNetWorkState()->SetMesPacket(charPos.y);
		id++;
	}
	IpNetWork->GetNetWorkState()->RevUpdate();

}

void GameScene::UpdateGuest()
{
	IpNetWork->GetNetWorkState()->RevUpdate();
	for (auto CHAR : character_)
	{
		
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
