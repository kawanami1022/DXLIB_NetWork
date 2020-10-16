#include <iostream>
#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Init;
    input_.moveDir = 0;
    controller_.Setup(0);
}

GuestNetWorkState::~GuestNetWorkState()
{
}

NetWorkMode GuestNetWorkState::GetNetWorkMode()
{
    return NetWorkMode::GUEST;
}

ActiveState GuestNetWorkState::ConnectHost(IPDATA hostIP)
{
    // ConnectNetWorkが成功していたらtrueにする
    netHandle = ConnectNetWork(hostIP);
    active_ = ActiveState::Init;
    if(netHandle!=-1)
    active_ = ActiveState::Stanby;

    return active_;
}

bool GuestNetWorkState::CheckNetWork()
{
    return false;
}

bool GuestNetWorkState::Update()
{
    controller_.Update();
    input_.moveDir = 0x00;
    if (controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.moveDir |= 0x01;
    }

    if (controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.moveDir |= 0x02;
    }

    if (controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Now)]==1&& controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Old)]==1)
    {
        //ビット演算
        input_.moveDir |= 0x04;

    }

    if (controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Old)] == 1)
    {
        //ビット演算
        input_.moveDir |= 0x08;

    }

    NetWorkSend(netHandle, &input_, sizeof(input_));


    return false;
}
