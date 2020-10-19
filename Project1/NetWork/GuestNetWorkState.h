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
	//�Q�b�^�[�Z�b�^�[

private:

	// �֐�
	void UpdateFuncWait();	// �ڑ��ҋ@���(�z�X�g�p)
	void UpdateFuncInit();	// ��������(�Q�[���J�n�������A�z�X�g�Q�X�g���p)
	void UpdateFuncStanby();	// ��������񑗐M�ς݂̊J�n�҂�
	void UpdateFuncPlay();	// �Q�[����(�z�X�g/�Q�X�g���p)
	void UpdateFuncOFFLINE();
};

