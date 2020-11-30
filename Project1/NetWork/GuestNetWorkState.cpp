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
    // ConnectNetWorkÇ™ê¨å˜ÇµÇƒÇ¢ÇΩÇÁtrueÇ…Ç∑ÇÈ
    netHandle.front().first = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle.front().first != -1)
    {
        // ê⁄ë±Ç≥ÇÍÇƒÇ‹ÇπÇÒ
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

    std::cout << "Init" << std::endl;
    std::thread func(&GuestNetWorkState::ReservMessageData, this);
    func.join();
    if (GetNetWorkDataLength(netHandle.front().first) >= sizeof(input_))
    {
        if (NetWorkRecv(netHandle.front().first, &input_, sizeof(input_)) == 0)
        {
            std::cout << "èâä˙âªèÓïÒÇéÛêMÇ‹ÇµÇΩ" << std::endl;
            //Ç±Ç±Ç…èâä˙âªèÓïÒÇì¸óÕÇ∑ÇÈ
        }
    }

   
    active_ = ActiveState::Stanby;
}

void GuestNetWorkState::UpdateFuncStanby()
{
    char mesType = 0;
    for (auto NetHandle : netHandle)
    {
        if (GetNetWorkDataLength(NetHandle.first) > 0)
        {
            NetWorkRecv(NetHandle.first, &mesType, sizeof(MesType));
            if (static_cast<MesType>(mesType) == MesType::STANBY)
            {
                active_ = ActiveState::Play;
            }
        }
    }
}

void GuestNetWorkState::UpdateFuncPlay()
{
   
    //std::cout << "----------------Play----------------" << std::endl;
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
