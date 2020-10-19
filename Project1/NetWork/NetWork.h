#pragma once
#include <memory>
#include <DxLib.h>

#include "NetWorkState.h"

enum class MesType
{
	STANBY,				// ��������񑗐M����
	GAME_START,			// �z�X�g���珉�������ł̏����������A�Q�[���J�n(
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
		//�Q�b�^�[�Z�b�^�[
	InputState GetInputState()
	{
		return state_->GetInputState();
	}
private:
	std::unique_ptr<NetWorkState> state_;

	static NetWork* hInstance;
	NetWork();
};

