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
    auto tmpID = GetNetWorkAcceptState(netHandle.front().first);
    if(tmpID==-1)
    {
        std::cout << "切断されてます" << std::endl;
        active_ = ActiveState::Init;
    }
    else {
        std::cout << "接続されてます" << std::endl;
        GetNetWorkIP(netHandle.front().first, &ipdata);
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

void HostNetWorkState::UpdateFuncNon()
{
}

void HostNetWorkState::UpdateFuncWait()
{
    auto handle = GetNewAcceptNetWork();
    if (handle !=-1)
    {
        netHandle.emplace_back(std::make_pair(handle,playerID_));
        playerID_++;
        std::cout << "接続されてます" << std::endl;
        active_ = ActiveState::Init;
    }

}

void HostNetWorkState::UpdateFuncInit()
{
    std::thread func(&HostNetWorkState::SendMessageData, this);

    std::cout << "初期化完了 !    Stanby状態に移動" << std::endl;
    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle.front().first);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle.front().first, &input_, sizeof(input_));
            std::cout << "取得したデータ" << std::setw(5) << input_.moveDir << std::endl;
        }
        func.join();
        active_ = ActiveState::Wait;
    }
   
}

void HostNetWorkState::UpdateFuncStanby()
{
    if (netHandle.size() <= 0)active_ = ActiveState::Stanby;
    Header header{MesType::STANBY_HOST,0,0,1 };
    std::cout <<"ネットワークハンドル数:"<< netHandle.size() << std::endl;
    for (auto NetHandle:netHandle)
    {
        std::cout << "STANBY_HOSTを送信します------" << std::endl;
        NetWorkSend(NetHandle.first, &header.data_[0], sizeof(int));
        NetWorkSend(NetHandle.first, &header.data_[1], sizeof(int));
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
