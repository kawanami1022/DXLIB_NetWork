#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver()
{
}

CrossOver::CrossOver(UniqueBase oldScene, UniqueBase nextScene)
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
    return std::move(nowScene);
}

void CrossOver::Draw()
{
    oldScene_->DrawOwnScreen();
    nextScene_->DrawOwnScreen();
}

void CrossOver::DrawOwnScreen()
{
}
