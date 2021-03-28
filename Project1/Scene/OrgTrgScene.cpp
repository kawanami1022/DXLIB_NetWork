#include <DxLib.h>
#include "OrgTrgScene.h"

OrgTrgScene::OrgTrgScene() :brightParam_(0), intervalTime_(0), mode_(DRAW_STATE::DRAW_STATE_OLD)
{
}

OrgTrgScene::OrgTrgScene(UniqueBase oldScene, UniqueBase nextScene) : brightParam_(0), intervalTime_(0), mode_(DRAW_STATE::DRAW_STATE_OLD)
{
	oldScene_ = std::move(oldScene);
	nextScene_ = std::move(nextScene);
}

OrgTrgScene::~OrgTrgScene()
{
}

UniqueBase OrgTrgScene::input(UniqueBase nowScene)
{
	return std::move(nowScene);
}

UniqueBase OrgTrgScene::UpDate(UniqueBase nowScene)
{
	brightParam_++;

	if (mode_ == DRAW_STATE::DRAW_STATE_OLD)
	{
		intervalTime_++;
		if (intervalTime_ == INTERVAL_FLAME * 8 * 6)
		{
			mode_ = DRAW_STATE::DRAW_STATE_NOW;
		}
	}
	if (mode_ == DRAW_STATE::DRAW_STATE_NOW)
	{
		intervalTime_--;
		if (intervalTime_ == 0)
		{
			nowScene = std::move(nextScene_);
			return  std::move(nowScene);
		}
	}

	Draw();

	return  std::move(nowScene);
}

void OrgTrgScene::Draw()
{
	ClsDrawScreen();
	if (mode_ == DRAW_STATE::DRAW_STATE_OLD)
	{
		oldScene_->Draw();
	}

	if (mode_ == DRAW_STATE::DRAW_STATE_NOW)
	{
		nextScene_->Draw();
	}
	SetDrawScreen(screenSrcID_);

	//oldScene_->DrawOwnScreen();
	nextScene_->DrawOwnScreen();
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (y * 8 + x <= intervalTime_ / INTERVAL_FLAME)
			{
				DrawBox(x * 100, y * 100, x * 100 + 100, y * 100 + 100, 0xffffff, true);
			}
		}
	}
}