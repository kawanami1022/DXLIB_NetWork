#pragma once
#include "NetWorkState.h"

class TMX_File;

class HostNetWorkState :
	public NetWorkState
{
public:
	HostNetWorkState();
	~HostNetWorkState();
	NetWorkMode GetNetWorkMode();
	bool CheckNetWork();
	bool Update();


protected:
private:
	// �֐�
	void UpdateFuncNon();
	void UpdateFuncWait();		// �ڑ��ҋ@���(�z�X�g�p)
	void UpdateFuncInit();		// ��������(�Q�[���J�n�������A�z�X�g�Q�X�g���p)
	void UpdateFuncStanby();	// ��������񑗐M�ς݂̊J�n�҂�
	void UpdateFuncPlay();		// �Q�[����(�z�X�g/�Q�X�g���p)
	void UpdateFuncOFFLINE();
};

