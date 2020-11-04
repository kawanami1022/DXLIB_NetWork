#include <DxLib.h>
#include "GameScene.h"

UniqueBase GameScene::input(UniqueBase nowScene)
{
	return nowScene;
}

UniqueBase GameScene::UpDate(UniqueBase nowScene)
{
	return nowScene;
}

void GameScene::Draw()
{
	DrawOwnScreen();
}

void GameScene::DrawOwnScreen()
{
	SetDrawScreen(screenSrcID_);
	ClsDrawScreen();

}

bool GameScene::Init()
{
	return false;
}
