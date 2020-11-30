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
    //�ڑ���Ԃ��擾����
    auto tmpID = GetNetWorkAcceptState(netHandle.front().first);
    if(tmpID==-1)
    {
        std::cout << "�ؒf����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }
    else {
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        GetNetWorkIP(netHandle.front().first, &ipdata);
        std::cout << "�ڑ���IP�A�h���X" <<std::setw(15)<< (int)(ipdata.d1)<<'.'<<
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
        netHandle.emplace_back(std::make_pair(handle,0));
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }
}

void HostNetWorkState::UpdateFuncInit()
{

    std::cout << "���������� !    Stanby��ԂɈړ�" << std::endl;

    active_ = ActiveState::Stanby;
}

void HostNetWorkState::UpdateFuncStanby()
{
    std::thread func(&HostNetWorkState::SendMessageData,this);

    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle.front().first);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle.front().first, &input_, sizeof(input_));
            std::cout << "�擾�����f�[�^" << std::setw(5) << input_.moveDir << std::endl;
        }
        func.join();
        active_ = ActiveState::Play;
    }
}

void HostNetWorkState::UpdateFuncPlay()
{
    //std::cout << "----------------Play----------------" << std::endl;
    if (GetLostNetWork() != -1)
    {
        std::cout << "�ڑ����؂�܂���!" << std::endl;
        //NetWorkSend(netHandle,)
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{

}
