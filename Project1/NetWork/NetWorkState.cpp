#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"
#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Init;
	input_.moveDir = 0;
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

ActiveState NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
