#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver():brightParam_(0)
{
}

CrossOver::CrossOver(UniqueBase oldScene, UniqueBase nextScene) : brightParam_(0)
{
    oldScene_ = std::move(oldScene);
    nextScene_ = std::move(nextScene);
}

CrossOver::~CrossOver()
{
}

UniqueBase CrossOver::input(UniqueBase nowScene)
{
    return std::move(nowScene);
}

UniqueBase CrossOver::UpDate(UniqueBase nowScene)
{
    brightParam_++;

    if (brightParam_ >= BRIGHT_MAX)
    {
        nowScene = std::move(nextScene_);
        return  std::move(nowScene);
    }
    Draw();
    
    return  std::move(nowScene);
}

void CrossOver::Draw()
{
    ClsDrawScreen();
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightParam_);
    nextScene_->Draw();
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, BRIGHT_MAX - brightParam_);
    oldScene_->Draw();
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, BRIGHT_MAX);
    SetDrawScreen(screenSrcID_);

    oldScene_->DrawOwnScreen();
    nextScene_->DrawOwnScreen();
}

