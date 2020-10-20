#include <memory>
#include <iostream>

#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Init;
    input_.moveDir = 0;
    controller_.Setup(0);
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
    // ConnectNetWork���������Ă�����true�ɂ���
    netHandle = ConnectNetWork(hostIP);
    active_ = ActiveState::Init;
    if(netHandle!=-1)
    active_ = ActiveState::Stanby;

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

void GuestNetWorkState::UpdateFuncNon()
{
}

void GuestNetWorkState::UpdateFuncWait()
{
    
}

void GuestNetWorkState::UpdateFuncInit()
{
    if (GetNetWorkDataLength(netHandle) >= sizeof(input_))
    {
        if (NetWorkRecv(netHandle, &input_, sizeof(input_)) == 0)
        {
            std::cout << "������������M�܂���" << std::endl;
            //�����ɏ�����������͂���
            active_ = ActiveState::Play;
        }
    }
   
}

void GuestNetWorkState::UpdateFuncStanby()
{
}

void GuestNetWorkState::UpdateFuncPlay()
{

    controller_.Update();
    input_.moveDir = 0x00;
    if (controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Old)] == 1)
    {
        //�r�b�g���Z
        input_.moveDir |= 0x01;
    }

    if (controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Old)] == 1)
    {
        //�r�b�g���Z
        input_.moveDir |= 0x02;
    }

    if (controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Old)] == 1)
    {
        //�r�b�g���Z
        input_.moveDir |= 0x04;
    }

    if (controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Old)] == 1)
    {
        //�r�b�g���Z
        input_.moveDir |= 0x08;
    }
    NetWorkSend(netHandle, &input_, sizeof(input_));
    if (GetLostNetWork()==0)
    {
        active_ = ActiveState::Init;
    }
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
