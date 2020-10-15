#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = false;
}

NetWorkState::~NetWorkState()
{
}

NetWorkMode NetWorkState::GetNetWorkMode()
{
	return NetWorkMode::OFFLINE;
}

bool NetWorkState::Update()
{
	return false;
}

bool NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
