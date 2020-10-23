#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <DxLib.h>

#include "NetWorkState.h"
#define IpNetWork	NetWork::Gethinstance()

enum class MesType
{
	STANBY,				// 初期化情報送信完了
	GAME_START,			// ホストから初期化情報での初期化完了、ゲーム開始(
	TMX_SIZE,
	POS
};

struct MesDate
{
	MesType type;
	int data[2];
};

using RevBox = std::vector<int>;


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
	void SendMessageData();
	void ReservMessageData();
		//ゲッターセッター
	InputState GetInputState()
	{
		return state_->GetInputState();
	}

	std::unordered_map< MesType, std::function<void(void)>> updateMesType_;

private:
	std::unique_ptr<NetWorkState> state_;
	int fileSize_;
	RevBox revdata_;
	static NetWork* hInstance;
	MesDate mesData_;
	NetWork();

	void STANBY();
	void GAME_START();
	void TMX_SIZE();
};

