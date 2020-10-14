#include <DxLib.h>
#include <iostream>
#include "HostNetWorkState.h"

HostNetWorkState::HostNetWorkState()
{
    active_ = !PreparationListenNetWork(portNum_);
    std::cout << active_ << "    " << portNum_<<"     ";
}

HostNetWorkState::~HostNetWorkState()
{
}

NetWorkMode HostNetWorkState::GetNetWorkMode()
{
    return NetWorkMode::HOST;
}
