#pragma once
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"


struct InputState
{
	unsigned int moveDir;
};

enum class NetWorkMode
{
	NON,
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

enum class ActiveState
{
	Non,		// ���ݒ�
	Wait,		// �ڑ��ҋ@���(�z�X�g�p)
	Init,		// ��������(�Q�[���J�n�������A�z�X�g�Q�X�g���p)
	Stanby,		// ��������񑗐M�ς݂̊J�n�҂�
	Play,		// �Q�[����(�z�X�g/�Q�X�g���p)
	OFFLINE
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
	ActiveState GetActive(void);
	const int GetPortNum() {
		return portNum_;
	}

	void SetNetHandle(int handle)
	{
		netHandle = handle;
	}
protected:
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle = 0;		// dxlib���g�p����n���h��
	KeyInput controller_;

	// �ϐ��錾
	InputState input_;
};

