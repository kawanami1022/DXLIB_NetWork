#include <filesystem>
#include <DxLib.h>
#include <iostream>
#include <iomanip>
#include <mutex>
#include "NetWork.h"
#include "HostNetWorkState.h"
#include "../Actor/Character/Character.h"
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

bool HostNetWorkState::CheckNetWork(int netHandle)
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

void HostNetWorkState::UpdateFuncNon()
{
}

void HostNetWorkState::UpdateFuncWait()
{
    auto handle = GetNewAcceptNetWork();
    if (handle !=-1)
    {
        playerID_ += UNIT_ID_BASE;
        netHandle.emplace_back(std::make_pair(handle,playerID_));
        std::cout << "接続されてます" << std::endl;
        active_ = ActiveState::Init;
    }

}

void HostNetWorkState::UpdateFuncInit()
{
    std::thread func([&]() {SendMessageData(netHandle.back().first); });

    std::cout << "初期化完了 !    Stanby状態に移動" << std::endl;
    if (CheckNetWork(netHandle.back().first))
    {
        func.join();
        std::cout << "-------------waitに戻る---------------" << std::endl;
    }
    else
    {
        // 接続されてない場合の処理
        netHandle.erase(std::next(netHandle.end(), 0));     // 挿入先の要素を削除する
    }
    active_ = ActiveState::Wait;
}

void HostNetWorkState::UpdateFuncStanby()
{

    Header header{MesType::STANBY_HOST,0,0,1 };
    auto SendData=[&]() {
        for (auto NetHandle : netHandle)
        {
            std::cout << "STANBY_HOSTを送信します------" << std::endl;
            NetWorkSend(NetHandle.first, &header, sizeof(MesHeader));
        }
        sendStanbyFlag = false;
    };

    if (netHandle.size() <= 0)active_ = ActiveState::Wait;
    std::cout <<"ネットワークハンドル数:"<< netHandle.size() << std::endl;
    if(sendStanbyFlag)SendData();

    for (auto NetHandle : netHandle)
    {
        if (0 != GetNetWorkDataLength(NetHandle.first))
        {
            NetWorkRecv(NetHandle.first, &header, sizeof(MesHeader));

            active_ = (header.mesdata_.type == MesType::STANBY_GUEST)?
                ActiveState::Play: ActiveState::Stanby;
        }
        else {
            active_ = ActiveState::Stanby;
        }
        if (active_ == ActiveState::Stanby)
        {
            sendStanbyFlag = true;
        }
        
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
