#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"
#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Init;
	input_.moveDir = 0;
	activeFunc_ = { {ActiveState::Wait,std::bind(&NetWorkState::UpdateFuncWait,this)},
				{ActiveState::Init,std::bind(&NetWorkState::UpdateFuncInit,this)},
				{ActiveState::Stanby,std::bind(&NetWorkState::UpdateFuncStanby,this)},
				{ActiveState::Play,std::bind(&NetWorkState::UpdateFuncPlay,this)},
				{ActiveState::OFFLINE,std::bind(&NetWorkState::UpdateFuncOFFLINE,this)} };
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

void NetWorkState::UpdateFuncWait()
{
}

void NetWorkState::UpdateFuncInit()
{
}

void NetWorkState::UpdateFuncStanby()
{
}

void NetWorkState::UpdateFuncPlay()
{
}

void NetWorkState::UpdateFuncOFFLINE()
{
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}
