#include <iostream>
#include <DxLib.h>
#include "../Lib/File/TMX_File.h"
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
	for (int y = 0; y < tmxFile_->height_; y++)
	{
		for (int x = 0; x < tmxFile_->width_; x++)
		{
			for (auto Name : tmxFile_->name_)
			{
				DrawExtendGraph(x * 32, y * 32, x * 32 + 32, y * 32 + 32, tileHandle_[tmxFile_->tiledMap_[Name].titleID_[x][y]], true);
			}
		}
	}

}


GameScene::GameScene()
{
	Init();
	//Draw();
}

GameScene::~GameScene()
{
}

bool GameScene::Init()
{
	tmxFile_ = IpNetWork->GetNetWorkState()->GetTMxData();
	tileHandle_ = std::make_unique<int[]>(12 + 1);
	tileHandle_[0] = -1;
	LoadDivGraph("Image/map.png", 12, 4, 3, tmxFile_->tileheight_, tmxFile_->tilewidth_, &tileHandle_[1]);


	for (int idx = 0; idx < 13; idx++)
	{
		std::cout << tileHandle_[idx] << std::endl;
	}

	return false;
}
