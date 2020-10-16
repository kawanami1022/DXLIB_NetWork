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
    // ConnectNetWorkが成功していたらtrueにする
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
    
    if (controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.move_way |= 0x01;
    }

    if (controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.move_way |= 0x02;
    }

    if (controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Now)]==1&& controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Old)]==1)
    {
        //ビット演算
        input_.move_way |= 0x04;

    }

    if (controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.move_way |= 0x08;

    }

    NetWorkSend(netHandle, &input_, sizeof(input_));


    return false;
}
