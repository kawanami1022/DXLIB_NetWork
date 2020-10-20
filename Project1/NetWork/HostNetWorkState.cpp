#include <DxLib.h>
#include <iostream>
#include <iomanip>
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
    //�ڑ���Ԃ��擾����
    auto tmpID = GetNetWorkAcceptState(netHandle);
    if(tmpID==-1)
    {
        std::cout << "�ؒf����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }
    else {
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        netHandle = tmpID;
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
    if (GetNewAcceptNetWork()==1)
    {
        std::cout << "�ڑ�����Ă܂�" << std::endl;
        active_ = ActiveState::Init;
    }
}

void HostNetWorkState::UpdateFuncInit()
{

    std::cout << "���������� !    Stanby��ԂɈړ�" << std::endl;
}

void HostNetWorkState::UpdateFuncStanby()
{
}

void HostNetWorkState::UpdateFuncPlay()
{
    if (CheckNetWork())
    {
        auto DataLength = GetNetWorkDataLength(netHandle);
        if (DataLength >= sizeof(input_))
        {
            NetWorkRecv(netHandle, &input_, sizeof(input_));
            std::cout << "�擾�����f�[�^" << std::setw(5) << input_.moveDir << std::endl;
        }
    }
}

void HostNetWorkState::UpdateFuncOFFLINE()
{
}
