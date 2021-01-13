#include <DxLib.h>
#include "LoginScene.h"
#include "CrossOver.h"
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
	std::memcpy(&keyIdNow_, &keyIdOld_, UCHAR_MAX);
	GetHitKeyStateAll(keyIdNow_.data());

	if (keyIdOld_[KEY_INPUT_RETURN]==1)
	{
		auto nextScene = std::make_unique<LoginScene>();
		nowScene = std::make_unique<CrossOver>(std::move(nowScene), std::move(nextScene));
	}

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
