#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

UniqueBase ResultScene::input(UniqueBase nowScene)
{
	return nowScene;
}

UniqueBase ResultScene::UpDate(UniqueBase nowScene)
{
	SetDrawScreen(screenSrcID_);
	ClsDrawScreen();
	Draw();
	return nowScene;
}

void ResultScene::Draw()
{
	auto idx = 0;
	auto result=IpNetWorkState->GetResult();
	for (auto RESULT : result)
	{
		DrawFormatString(0, idx * 16, 0xffffff, "ID:%d", RESULT);
		idx++;
	}
}

void ResultScene::UpdateHost()
{
}

void ResultScene::UpdateGuest()
{
}

void ResultScene::UpdateOFFLINE()
{
}

void ResultScene::Network()
{
}
