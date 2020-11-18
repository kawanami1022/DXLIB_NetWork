#include <memory>
#include <iostream>

#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Non;
    input_.moveDir = 0;
    controller_.Setup(0);
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
    netHandle = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle != -1)
    {
        // 接続されてません
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

bool GuestNetWorkState::RevUpdate()
{
    dataPacket_.clear();    
    int revData = 0;
    if (GetLostNetWork() != -1)
    {
        std::cout << "接続が切れてます!" << std::endl;
        return false;
    }

    // ネットワークバッファに溜まっているデータが存在するかくにんする
    while (GetNetWorkDataLength(netHandle) > 0)
    {
        NetWorkRecv(netHandle, &revData, sizeof(int));
        dataPacket_.emplace_back(revData);
    }

    for (auto REVDATA : dataPacket_)
    {
        std::cout << "受け取ったデータ:" << std::hex << REVDATA << std::endl;
    }

    return true;
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
            std::cout << "初期化情報を受信ました" << std::endl;
            //ここに初期化情報を入力する
            active_ = ActiveState::Play;
        }
    }
}

void GuestNetWorkState::UpdateFuncPlay()
{
    std::cout << "----------------Play----------------" << std::endl;
    controller_.Update();
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
