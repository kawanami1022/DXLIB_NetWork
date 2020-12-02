#include <filesystem>
#include <DxLib.h>
#include <iostream>
#include <iomanip>
#include <mutex>
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
        netHandle.emplace_back(std::make_pair(handle,playerID_));
        playerID_++;
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }

}

void HostNetWorkState::UpdateFuncInit()
{
    std::thread func(&HostNetWorkState::SendMessageData, this);

    std::cout << "���������� !    Stanby��ԂɈړ�" << std::endl;
    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle.front().first);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle.front().first, &input_, sizeof(input_));
            std::cout << "�擾�����f�[�^" << std::setw(5) << input_.moveDir << std::endl;
        }
        func.join();
        active_ = ActiveState::Wait;
    }
   
}

void HostNetWorkState::UpdateFuncStanby()
{

    Header header{MesType::STANBY_HOST,0,0,1 };
    auto SendData=[&]() {
        for (auto NetHandle : netHandle)
        {
            std::cout << "STANBY_HOST�𑗐M���܂�------" << std::endl;
            NetWorkSend(NetHandle.first, &header.data_[0], sizeof(int));
            NetWorkSend(NetHandle.first, &header.data_[1], sizeof(int));
        }
    };

    if (netHandle.size() <= 0)active_ = ActiveState::Wait;
    std::cout <<"�l�b�g���[�N�n���h����:"<< netHandle.size() << std::endl;
    static std::once_flag once;
    std::call_once(once, SendData);

    for (auto NetHandle : netHandle)
    {
        if (0 != GetNetWorkDataLength(NetHandle.first))
        {
            NetWorkRecv(NetHandle.first, &header.data_[0], sizeof(int));
            NetWorkRecv(NetHandle.first, &header.data_[1], sizeof(int));

            active_ = (header.mesdata_.type == MesType::STANBY_GUEST)?
                ActiveState::Play: ActiveState::Stanby;
        }
        else {
            active_ = ActiveState::Stanby;
        }
        
    }

}

void HostNetWorkState::UpdateFuncPlay()
{
    std::cout << "----------------Play----------------" << std::endl;
    if (GetLostNetWork() != -1)
    {
        std::cout << "�ڑ����؂�܂���!" << std::endl;
        //NetWorkSend(netHandle,)
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{

}
