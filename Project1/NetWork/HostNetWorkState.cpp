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
    playerMax_ = 1;
    mesData_.sendID = 0;
    isSetSendStart_ = false;
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
    //�ڑ���Ԃ��擾����
    auto tmpID = GetNetWorkAcceptState(netHandle);
    if(tmpID==-1)
    {
        std::cout << "�ؒf����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }
    else {
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        GetNetWorkIP(netHandle, &ipdata);
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

        playerID_ += UNIT_ID_BASE;
        playerMax_++;
        netHandle.emplace_back(std::make_pair(handle,playerID_));
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        // ID�𑗐M����
        Header header{ MesType::ID,0,0,2 };

        std::vector<int> data = { header.data_[0],header.data_[1],playerID_,playerMax_ };
        for (auto& DATA : data)
        {
            SetSendPacket(DATA);
        }
        data.clear();
        SendUpdate(netHandle.back());
        if (!isSetSendStart_)
        {
            startTime_ = std::chrono::system_clock::now() + std::chrono::seconds(30);//15�b��̊J�n�������擾����
            isSetSendStart_ = true;
        }
        header={ MesType::COUNT_DOWN_ROOM,0,0,2};
        data = { header.data_[0],header.data_[1] };
        header.start_ = startTime_;
        data.emplace_back(header.data_[0]);
        data.emplace_back(header.data_[1]);
        for (auto& DATA : data)
        {
            SetSendPacket(DATA);
        }
        SendUpdate(netHandle.back());
        //COUNT_DOWN_GAME,		//�S���̏������������̃Q�[���J�n���� {MesType �w�b�_�[,longlong ����}
        
        active_ = ActiveState::Init;
    }

}

void HostNetWorkState::UpdateFuncInit()
{

    std::cout << "���������� !    Stanby��ԂɈړ�" << std::endl;
    if (CheckNetWork(netHandle.back().first))
    {
        std::cout << "-------------wait�ɖ߂�---------------" << std::endl;
    }
    else
    {
        // �ڑ�����ĂȂ��ꍇ�̏���
        netHandle.erase(std::next(netHandle.end(), 0));     // �}����̗v�f���폜����
    }
    active_ = ActiveState::Wait;
}

void HostNetWorkState::UpdateFuncStanby()
{

    Header header{MesType::STANBY_HOST,0,0,1 };
    auto SendData=[&]() {
        for (auto NetHandle : netHandle)
        {
            NetWorkSend(NetHandle.first, &header, sizeof(MesHeader));
        }
        sendStanbyFlag = false;
    };

    if (netHandle.size() <= 0)active_ = ActiveState::Wait;
    std::cout <<"�l�b�g���[�N�n���h����:"<< netHandle.size() << std::endl;
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
        std::cout << "�ڑ����؂�܂���!" << std::endl;
        //NetWorkSend(netHandle,)
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{

}
