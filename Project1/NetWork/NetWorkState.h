#pragma once
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"
struct InputState
{
	int move_way;
};

enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual bool Update();
	bool ConnectHost(IPDATA hostIP);
	//virtual bool CheckNetWork() = 0;

	//�Q�b�^�[�Z�b�^�[
	virtual NetWorkMode GetNetWorkMode();
	InputState GetInputState()
	{
		return input_;
	}
	bool GetActive(void);
protected:
	const int portNum_ = 8086;
	bool active_;
	int netHandle = 0;		// dxlib���g�p����n���h��
	KeyInput controller_;


	// �ϐ��錾
	InputState input_;
};

