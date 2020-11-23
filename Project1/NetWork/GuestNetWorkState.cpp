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
    // ConnectNetWork‚ª¬Œ÷‚µ‚Ä‚¢‚½‚çtrue‚É‚·‚é
    netHandle = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle != -1)
    {
        // Ú‘±‚³‚ê‚Ä‚Ü‚¹‚ñ
        active_ = ActiveState::Non;
    }
    std::cout << netHandle << std::endl;
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
    active_ = ActiveState::Stanby;
}

void GuestNetWorkState::UpdateFuncStanby()
{
    std::thread func(&GuestNetWorkState::ReservMessageData,this);
    func.join();
    if (GetNetWorkDataLength(netHandle) >= sizeof(input_))
    {
        if (NetWorkRecv(netHandle, &input_, sizeof(input_)) == 0)
        {
            std::cout << "‰Šú‰»î•ñ‚ğóM‚Ü‚µ‚½" << std::endl;
            //‚±‚±‚É‰Šú‰»î•ñ‚ğ“ü—Í‚·‚é
            active_ = ActiveState::Play;
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
