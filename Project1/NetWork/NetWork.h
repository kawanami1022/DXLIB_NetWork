#pragma once
#include <memory>
#include <DxLib.h>
#include "NetWorkState.h"

#define IpNetWork	NetWork::Gethinstance()
class NetWork
{
public:
	static NetWork* Gethinstance()
	{
		Create();
		return hInstance;
	}

	static NetWork& Create()
	{
		if (hInstance == nullptr)
		{
			hInstance = new NetWork();
		}
		return (*hInstance);
	}

	static void Destroy()
	{
		delete hInstance;
		hInstance = nullptr;
	}

	IPDATA GetIP();


	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode();
	bool GetActice();

	bool ConnectHost(IPDATA hostIP);
private:
	std::unique_ptr<NetWorkState> state_;

	static NetWork* hInstance;
	NetWork();
};

