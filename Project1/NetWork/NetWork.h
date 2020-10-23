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
	STANBY,				// ��������񑗐M����
	GAME_START,			// �z�X�g���珉�������ł̏����������A�Q�[���J�n(
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
		//�Q�b�^�[�Z�b�^�[
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

