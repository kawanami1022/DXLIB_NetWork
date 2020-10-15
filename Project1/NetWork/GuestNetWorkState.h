#pragma once
#include "NetWorkState.h"
class GuestNetWorkState :
	public NetWorkState
{
public:
	GuestNetWorkState();
	~GuestNetWorkState();
	NetWorkMode GetNetWorkMode();
	bool ConnectHost(IPDATA hostIP);
	bool CheckNetWork();
	bool Update();
private:
	// •Ï”éŒ¾
	InputState input_;
};

