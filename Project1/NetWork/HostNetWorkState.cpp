#include <filesystem>
#include <DxLib.h>
#include <iostream>
#include <iomanip>
#include "NetWork.h"
#include "HostNetWorkState.h"

HostNetWorkState::HostNetWorkState()
{

    auto succeed = !PreparationListenNetWork(portNum_);
    if (succeed) { active_ = ActiveState::Wait; }
    std::cout << static_cast<int>(active_) << "    " << portNum_<<"     ";

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
        netHandle = tmpID;
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
    //MesDate data = { MesType::TMX_SIZE,{static_cast<int>(std::filesystem::file_size("map.tmx")) ,0} };

    //NetWorkSend(netHandle, &data, sizeof(MesDate));

    //std::cout << "MessageDataを送りました!" << std::endl;
    //std::cout << data.data[0] <<"byte"<< std::endl;
    active_ = ActiveState::Play;
}

void HostNetWorkState::UpdateFuncPlay()
{
    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle, &input_, sizeof(input_));
            std::cout << "取得したデータ" << std::setw(5) << input_.moveDir << std::endl;
        }
        ReservMessageData();
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{

}
