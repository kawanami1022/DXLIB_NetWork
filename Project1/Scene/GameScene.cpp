#include <iostream>
#include <DxLib.h>
#include "../Lib/File/TMX_File.h"
#include "../Actor/map/map.h"
#include "../Actor/Character/Character.h"
#include "GameScene.h"
UniqueBase GameScene::input(UniqueBase nowScene)
{
	return nowScene;

}

UniqueBase GameScene::UpDate(UniqueBase nowScene)
{
	SetDrawScreen(screenSrcID_);
	Draw();
	return nowScene;
}

void GameScene::Draw()
{

	SetDrawScreen(screenSrcID_);
	//ClsDrawScreen();
	map_->Draw();
	for (auto CHARACTER : character_)
	{
		CHARACTER->Draw();
	}

}


GameScene::GameScene()
{
	Init();
	Draw();

}

GameScene::~GameScene()
{
}

bool GameScene::Init()
{
	map_=std::make_unique<Map>();

	// sponePlayer

	VecCharacter sponePlayer= map_->SponePlayer();
	character_.insert(character_.end(), sponePlayer.begin(), sponePlayer.end());
	return false;
}
