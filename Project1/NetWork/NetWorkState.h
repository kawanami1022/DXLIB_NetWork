#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"

namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}


class Timer;
enum class ActiveState;

using ActiveFunc = std::unordered_map< ActiveState, std::function<void(void)>>;
using RevBox = std::vector<char>;
enum class MesType
{
	INIT,
	STANBY,				// ��������񑗐M����
	TMX_SIZE,
	TMX_DATA,
	GAME_START,			// �z�X�g���珉�������ł̏����������A�Q�[���J�n(
	POS
};

struct MesDate
{
	MesType type;
	unsigned int data[2];
};


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
	void SendMessageData();
	void ReservMessageData();

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

	void SetTMXData(std::shared_ptr<File::TMX_File> tmxdata)
	{
		tmxFile_ = tmxdata;
	}

protected:

	// �ϐ��錾
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle = 0;		// dxlib���g�p����n���h��
	KeyInput controller_;

	InputState input_;
	ActiveFunc activeFunc_;
	
	int fileSize_;
	RevBox revdata_;
	MesDate mesData_;

	std::unordered_map< MesType, std::function<void(void)>> updateMesType_;
	std::unique_ptr<Timer> timer_;
	std::shared_ptr<File::TMX_File> tmxFile_=nullptr;

	// �֐�
	virtual void UpdateFuncNon();	
	virtual void UpdateFuncWait();	// �ڑ��ҋ@���(�z�X�g�p)
	virtual void UpdateFuncInit();	// ��������(�Q�[���J�n�������A�z�X�g�Q�X�g���p)
	virtual void UpdateFuncStanby();	// ��������񑗐M�ς݂̊J�n�҂�
	virtual void UpdateFuncPlay();	// �Q�[����(�z�X�g/�Q�X�g���p)
	virtual void UpdateFuncOFFLINE();

	void STANBY();
	void GAME_START();
	void TMX_SIZE();
};

