#pragma once
#include <thread>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <DxLib.h>
#include "NetWorkState.h"
#define IpNetWork	NetWork::Gethinstance()
#define IpNetWorkState	NetWork::Gethinstance()->GetNetWorkState()
class NetWorkState;

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


	void Update();

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode();
	ActiveState GetActive();
	ActiveState ConnectHost(IPDATA hostIP);

	void RunUpdate(void);

		//ゲッターセッター
	InputState GetInputState()
	{
		return state_->GetInputState();
	}
	std::shared_ptr<NetWorkState> GetNetWorkState()
	{
		return state_;
	}


private:
	std::shared_ptr<NetWorkState> state_;

	static NetWork* hInstance;
	NetWork();


};

