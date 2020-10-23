#include <fstream>
#include <filesystem>
#include <iostream>
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "NetWork.h"
#include "HostNetWorkState.h"
#include "GuestNetWorkState.h"

NetWork* NetWork::hInstance = nullptr;

IPDATA NetWork::GetIP()
{
	IPDATA ipData;
	GetMyIPAddress(&ipData);
	return ipData;
}

void NetWork::Update()
{
	
	state_->Update();
}

bool NetWork::SetNetWorkMode(NetWorkMode mode)
{

	if (mode == NetWorkMode::NON) { state_ = std::make_unique<NetWorkState>(); }
	else if (mode == NetWorkMode::OFFLINE) { state_ = std::make_unique<NetWorkState>(); }
	else if (mode == NetWorkMode::HOST) { state_ = std::make_unique<HostNetWorkState>(); }
	else if (mode == NetWorkMode::GUEST) { state_ = std::make_unique<GuestNetWorkState>(); }
	else {
#ifdef _DEBUG
		if (mode != NetWorkMode::OFFLINE)
		{
			std::cout << "ネットワークのエラー検出" << std::endl;
		}
#endif
	}
	return false;
}

NetWorkMode NetWork::GetNetWorkMode()
{
	return state_->GetNetWorkMode();
}

ActiveState NetWork::GetActive()
{
	return state_->GetActive();
}

ActiveState NetWork::ConnectHost(IPDATA hostIP)
{
	//192.20.41.197
	//172.20.39.53
	std::cout << "------------NetWork class-----------" << std::endl;
	auto handle = ConnectNetWork(hostIP, state_->GetPortNum());
	
	if (handle != -1) {
		std::cout << "接続成功!" << std::endl;
		std::cout << handle << std::endl;
		state_->SetNetWorkState(ActiveState::Init);		// Init状態に変更
		state_->SetNetHandle(handle);
		return state_->GetActive();
	}
	std::cout << handle << std::endl;
	return state_->GetActive();
}

void NetWork::SendMessageData()
{
	NetWorkSend(state_->GetNetWorkHandle(), &mesData_, sizeof(mesData_));
}

void NetWork::ReservMessageData()
{
	NetWorkRecv(state_->GetNetWorkHandle(), &mesData_, sizeof(mesData_));
	std::cout << "fileSize:" << mesData_.data[0] << std::endl;
}

NetWork::NetWork()
{
	mesData_.type = MesType::TMX_SIZE;
	mesData_.data[0] = 0;
	mesData_.data[1] = 0;

	updateMesType_ =
	{ {MesType::TMX_SIZE ,std::bind(&NetWork::TMX_SIZE,this)},
		{MesType::GAME_START ,std::bind(&NetWork::GAME_START,this)},
		{MesType::STANBY ,std::bind(&NetWork::STANBY,this)} };

	std::cout << "OFFLINEに設定されてます" << std::endl;
	state_ = std::make_unique<NetWorkState>();
}

void NetWork::STANBY()
{
}

void NetWork::GAME_START()
{
}

void NetWork::TMX_SIZE()
{
	// ファイルサイズを取得する
	fileSize_ = static_cast<int>(std::filesystem::file_size("map.tmx"));
	std::cout << "fileSize:" << fileSize_ << std::endl;
	mesData_.type = MesType::STANBY;
	mesData_.data[0] = fileSize_;
}
