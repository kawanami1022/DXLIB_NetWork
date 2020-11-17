#include <memory>
#include <iostream>

#include "GuestNetWorkState.h"

GuestNetWorkState::GuestNetWorkState()
{
    active_ = ActiveState::Non;
    input_.moveDir = 0;
    controller_.Setup(0);
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
    // ConnectNetWorkが成功していたらtrueにする
    netHandle = ConnectNetWork(hostIP);

    active_ = ActiveState::Init;
    if (netHandle != -1)
    {
        // 接続されてません
        active_ = ActiveState::Non;
    }
    std::cout << netHandle << std::endl;
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

bool GuestNetWorkState::RevUpdate()
{
    if (GetLostNetWork() != -1)
    {
        return false;
    }
    return true;
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
    active_ = ActiveState::Stanby;
}

void GuestNetWorkState::UpdateFuncStanby()
{
    std::thread func(&GuestNetWorkState::ReservMessageData,this);
    func.join();
    if (GetNetWorkDataLength(netHandle) >= sizeof(input_))
    {
        if (NetWorkRecv(netHandle, &input_, sizeof(input_)) == 0)
        {
            std::cout << "初期化情報を受信ました" << std::endl;
            //ここに初期化情報を入力する
            active_ = ActiveState::Play;
        }
    }
}

void GuestNetWorkState::UpdateFuncPlay()
{
    std::cout << "----------------Play----------------" << std::endl;
    controller_.Update();
    //input_.moveDir = 0x00;
    //if (controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Up][static_cast<int>(Trg::Old)] == 1)
    //{
    //    //ビット演算
    //    input_.moveDir |= 0x01;
    //}

    //if (controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Right][static_cast<int>(Trg::Old)] == 1)
    //{
    //    //ビット演算
    //    input_.moveDir |= 0x02;
    //}

    //if (controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Down][static_cast<int>(Trg::Old)] == 1)
    //{
    //    //ビット演算
    //    input_.moveDir |= 0x04;
    //}

    //if (controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Now)] == 1 && controller_.GetCntData()[InputID::Left][static_cast<int>(Trg::Old)] == 1)
    //{
    //    //ビット演算
    //    input_.moveDir |= 0x08;
    //}
    //NetWorkSend(netHandle, &input_, sizeof(input_));
    //if (GetLostNetWork()==0)
    //{
    //    active_ = ActiveState::Init;
    //}
}

void GuestNetWorkState::UpdateFuncOFFLINE()
{
}
