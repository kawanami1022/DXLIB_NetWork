#include <DxLib.h>
#include <iostream>
#include <iomanip>
#include "HostNetWorkState.h"

HostNetWorkState::HostNetWorkState()
{
    active_ = !PreparationListenNetWork(portNum_);
    std::cout << active_ << "    " << portNum_<<"     ";

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
    auto tmpID = GetNewAcceptNetWork();
    if(tmpID==-1)
    {
        std::cout << "切断されてます" << std::endl;
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
    if (CheckNetWork())
    {
        GetNetWorkDataLength(netHandle);
        NetWorkRecv(netHandle, &input_, sizeof(input_));
        std::cout << "取得したデータ" << std::setw(5) << input_.move_way << std::endl;
    }
    return false;
}
