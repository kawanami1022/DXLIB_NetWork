#pragma once
#include <functional>
#include <unordered_map>
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"

enum class ActiveState;

using ActiveFunc = std::unordered_map< ActiveState, std::function<void(void)>>;


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
	ActiveState ConnectHost(IPDATA hostIP);
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
	int GetNetWorkHandle()
	{
		return netHandle;
	}

	void SetNetWorkState(ActiveState activeState)
	{
		this->active_ = activeState;
	}

	void SetNetHandle(int handle)
	{
		netHandle = handle;
	}
protected:

	// �ϐ��錾
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle = 0;		// dxlib���g�p����n���h��
	KeyInput controller_;

	InputState input_;
	ActiveFunc activeFunc_;


	// �֐�
	virtual void UpdateFuncNon();	
	virtual void UpdateFuncWait();	// �ڑ��ҋ@���(�z�X�g�p)
	virtual void UpdateFuncInit();	// ��������(�Q�[���J�n�������A�z�X�g�Q�X�g���p)
	virtual void UpdateFuncStanby();	// ��������񑗐M�ς݂̊J�n�҂�
	virtual void UpdateFuncPlay();	// �Q�[����(�z�X�g/�Q�X�g���p)
	virtual void UpdateFuncOFFLINE();

};

