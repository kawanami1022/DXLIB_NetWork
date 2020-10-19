#pragma once
#include <memory>
#include <DxLib.h>

#include "NetWorkState.h"

enum class MesType
{
	STANBY,				// 初期化情報送信完了
	GAME_START,			// ホストから初期化情報での初期化完了、ゲーム開始(
	POS
};

struct MesDate
{
	MesType type;
	int data[2];
};


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


	void Update();

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode();
	ActiveState GetActice();

	ActiveState ConnectHost(IPDATA hostIP);
		//ゲッターセッター
	InputState GetInputState()
	{
		return state_->GetInputState();
	}
private:
	std::unique_ptr<NetWorkState> state_;

	static NetWork* hInstance;
	NetWork();
};

