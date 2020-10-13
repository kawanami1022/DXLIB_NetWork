#include <iostream>
#include <DxLib.h>
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
TitleScene::TitleScene():BaseScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{


	GetDrawScreenSize(&screen_size_x, &screen_size_y);

	auto ipData = IpNetWork->GetIP();
	std::cout <<(int)(ipData.d1) << '.' << (int)(ipData.d2) << '.' << (int)(ipData.d3) << '.' << (int)(ipData.d4) << std::endl;
}

UniqueBase TitleScene::input(UniqueBase nowScene)
{
	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	return nowScene;
}

void TitleScene::Draw()
{
}
