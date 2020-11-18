#include <filesystem>
#include <DxLib.h>
#include <iostream>
#include <iomanip>
#include "NetWork.h"
#include "HostNetWorkState.h"

HostNetWorkState::HostNetWorkState()
{

    auto succeed = PreparationListenNetWork(portNum_);
    if (succeed==0) { active_ = ActiveState::Wait; }
    std::cout << static_cast<int>(active_) << "    " << portNum_<<"     ";
  
    mesData_.sendID = 0;
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
    IPDATA ipdata = { 0,0,0,0 };
    //接続状態を取得する
    auto tmpID = GetNetWorkAcceptState(netHandle);
    if(tmpID==-1)
    {
        std::cout << "切断されてます" << std::endl;
        active_ = ActiveState::Init;
    }
    else {
        std::cout << "接続されてます" << std::endl;
        GetNetWorkIP(netHandle, &ipdata);
        std::cout << "接続先IPアドレス" <<std::setw(15)<< (int)(ipdata.d1)<<'.'<<
            (int)(ipdata.d2) << '.'<< (int)(ipdata.d3) << '.'<< (int)(ipdata.d4) << '.'<<std::endl;
        return true;
    }
    return false;
}

bool HostNetWorkState::Update()
{
    activeFunc_[active_]();
    return false;
}

bool HostNetWorkState::RevUpdate()
{
    
    if (GetLostNetWork() != -1)
    {
        return false;
    }
    NetWorkSend(netHandle, dataPacket_.data(), dataPacket_.size() * sizeof(int));

    for (auto DATAPACKET : dataPacket_)
    {
        std::cout << "送ったデータ:" << std::hex << DATAPACKET << std::endl;
    }
    return true;
}

void HostNetWorkState::UpdateFuncNon()
{
}

void HostNetWorkState::UpdateFuncWait()
{
    if ((netHandle = GetNewAcceptNetWork())!=-1)
    {
        std::cout << "接続されてます" << std::endl;
        active_ = ActiveState::Init;
    }
}

void HostNetWorkState::UpdateFuncInit()
{

    std::cout << "初期化完了 !    Stanby状態に移動" << std::endl;

    active_ = ActiveState::Stanby;
}

void HostNetWorkState::UpdateFuncStanby()
{
    std::thread func(&HostNetWorkState::SendMessageData,this);

    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle, &input_, sizeof(input_));
            std::cout << "取得したデータ" << std::setw(5) << input_.moveDir << std::endl;
        }
        func.join();
        active_ = ActiveState::Play;
    }
}

void HostNetWorkState::UpdateFuncPlay()
{
    std::cout << "----------------Play----------------" << std::endl;
    if (GetLostNetWork() != -1)
    {
        std::cout << "接続が切れました!" << std::endl;
        //NetWorkSend(netHandle,)
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{

}
