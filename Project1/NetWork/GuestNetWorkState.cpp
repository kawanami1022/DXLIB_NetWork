#include <memory>
#include <iostream>
#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Non;
    input_.moveDir = 0;
    mesData_.sendID = 0;

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
    netHandle.front().first = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle.front().first != -1)
    {
        // 接続されてません
        active_ = ActiveState::Non;
    }
    std::cout << netHandle.front().first << std::endl;
    return active_;
}

bool GuestNetWorkState::CheckNetWork()
{
    return false;
}

bool GuestNetWorkState::Update()
{
    activeFunc_[active_]();
    return false;
}

void GuestNetWorkState::UpdateFuncNon()
{
    std::cout << "NON" << std::endl;
}

void GuestNetWorkState::UpdateFuncWait()
{
    std::cout << "Wait" << std::endl;
}

void GuestNetWorkState::UpdateFuncInit()
{
}

void GuestNetWorkState::UpdateFuncStanby()
{

    std::cout << "Stanby" << std::endl;
    Header headerData = { MesType::NON,0,0,1 };
    for (auto NetHandle : netHandle)
    {
        if (GetNetWorkDataLength(NetHandle.first) > 0)
        {
            // スタンバイモードなのかしかめる
            NetWorkRecv(NetHandle.first, &headerData, sizeof(MesHeader));
            if (headerData.mesdata_.type == MesType::STANBY_HOST)
            {
                headerData = { MesType::STANBY_GUEST,0,0,1 };
                std::cout << "データ受け取りました" << std::endl;
                NetWorkSend(NetHandle.first, &headerData, sizeof(MesHeader));
                active_ = ActiveState::Play;
            }
        }
    }
}

void GuestNetWorkState::UpdateFuncPlay()
{
   
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
