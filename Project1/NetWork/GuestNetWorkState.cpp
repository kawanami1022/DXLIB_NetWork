#include <iostream>
#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = false;
    input_.move_way = 0;
    controller_.Setup(0);
}

GuestNetWorkState::~GuestNetWorkState()
{
}

NetWorkMode GuestNetWorkState::GetNetWorkMode()
{
    return NetWorkMode::GUEST;
}

bool GuestNetWorkState::ConnectHost(IPDATA hostIP)
{
    // ConnectNetWorkÇ™ê¨å˜ÇµÇƒÇ¢ÇΩÇÁtrueÇ…Ç∑ÇÈ
    netHandle = ConnectNetWork(hostIP);
    active_ = false;
    if(netHandle!=-1)
    active_ = true;

    return active_;
}

bool GuestNetWorkState::CheckNetWork()
{
    return false;
}

bool GuestNetWorkState::Update()
{
    controller_.Update();
    controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Now)];
    //if ()
    //{

    //}
    NetWorkSend(netHandle, &input_, sizeof(input_));


    return false;
}
