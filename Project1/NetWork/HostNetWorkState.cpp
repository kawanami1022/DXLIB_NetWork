#include <DxLib.h>
#include <iostream>
#include "HostNetWorkState.h"

HostNetWorkState::HostNetWorkState()
{
    active_ = !PreparationListenNetWork(portNum_);
    std::cout << active_ << "    " << portNum_<<"     ";

    controller_.Setup(0);
}

HostNetWorkState::~HostNetWorkState()
{
}

NetWorkMode HostNetWorkState::GetNetWorkMode()
{
    return NetWorkMode::HOST;
}

bool HostNetWorkState::CheckNetWork()
{
    auto tmpID = GetNewAcceptNetWork();

    return false;
}
