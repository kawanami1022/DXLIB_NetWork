#include <memory>
#include <iostream>

#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Non;
    input_.moveDir = 0;
    mesData_.sendID = 0;

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
    netHandle.front().first = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle.front().first != -1)
    {
        // �ڑ�����Ă܂���
        active_ = ActiveState::Non;
    }
    std::cout << netHandle.front().first << std::endl;
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
    std::cout << "NON" << std::endl;
}

void GuestNetWorkState::UpdateFuncWait()
{
    std::cout << "Wait" << std::endl;
}

void GuestNetWorkState::UpdateFuncInit()
{

    std::cout << "Init" << std::endl;
    auto length = GetNetWorkDataLength(netHandle.front().first);
    if (length <= 0)return;
    //std::cout << "������������M�܂���" << std::endl;
    ////�����ɏ�����������͂���
    //active_ = ActiveState::Stanby;

   
}

void GuestNetWorkState::UpdateFuncStanby()
{
    char mesType = 0;
    for (auto NetHandle : netHandle)
    {
        if (GetNetWorkDataLength(NetHandle.first) > 0)
        {
            // �X�^���o�C���[�h�Ȃ̂������߂�
            NetWorkRecv(NetHandle.first, &mesType, sizeof(MesType));
            if (mesType == static_cast<char>(MesType::STANBY_GUEST))
            {
                active_ = ActiveState::Play;
            }
        }
    }
}

void GuestNetWorkState::UpdateFuncPlay()
{
   
    //std::cout << "----------------Play----------------" << std::endl;
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
