#pragma once
#include "NetWorkState.h"
class GuestNetWorkState :
	public NetWorkState
{
public:
	GuestNetWorkState();
	~GuestNetWorkState();
	NetWorkMode GetNetWorkMode();
	ActiveState ConnectHost(IPDATA hostIP);
	bool CheckNetWork();
	bool Update();
	//ゲッターセッター

private:

};

