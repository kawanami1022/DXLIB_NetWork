#include <DxLib.h>
#include "BaseScene.h"

BaseScene::BaseScene()
{
	screenSrcID_ = MakeScreen(800, 600,true);
}

BaseScene::~BaseScene()
{
}

void BaseScene::DrawOwnScreen(int screenID)
{
	DrawGraph(0, 0, screenID, true);
}

void BaseScene::DrawOwnScreen()
{
	DrawGraph(0, 0, screenSrcID_, true);
}


