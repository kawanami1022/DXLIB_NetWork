
#include <sstream>
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

void NetWork::RunUpdate(void)
{
}


NetWork::NetWork()
{


	//std::cout << "OFFLINEに設定されてます" << std::endl;
	state_ = std::make_unique<NetWorkState>();
}

