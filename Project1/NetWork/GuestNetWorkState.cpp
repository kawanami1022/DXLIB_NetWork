#include <iostream>
#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = false;
}

GuestNetWorkState::~GuestNetWorkState()
{
}

NetWorkMode GuestNetWorkState::GetNetWorkMode()
{
    return NetWorkMode::HOST;
}

bool GuestNetWorkState::ConnectHost(IPDATA hostIP)
{
    // ConnectNetWorkが成功していたらtrueにする
    netHandle = ConnectNetWork(hostIP);
    
    active_ = true;

    return active_;
}
