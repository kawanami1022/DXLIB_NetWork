#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <thread>
#include <utility>
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"

constexpr	auto MTU = 1454;
constexpr auto MESHEADER_INT = 3;
namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

class Timer;
enum class ActiveState;
struct MesHeader;
union unionData;
union Header;
enum class MesType :char;

using ActiveFunc = std::unordered_map< ActiveState, std::function<void(void)>>;
using RevBox = std::vector<char>;
using MesPacket = std::vector<int>;
using RevPacket = std::pair<MesType, std::vector<int>>;
using MesPacketList = std::vector<int>;
using ListInt = std::list<std::pair<int, unsigned int>>;		//int �l�b�g�n���h��:unsigned int :�v���[���[ID

enum class MesType :char
{
	NON = 100,
	COUNT_DOWN,		// �ڑ���t�J�E���g�_�E��
	ID,						// ������ID�ƃv���[���[�̑���
	STANBY,				// ��������񑗐M����
	GAME_START,		// �z�X�g���珉�������ł̏����������A�Q�[���J�n(
	TMX_SIZE,				
	TMX_DATA,			// 
	POS,						// �Q�[�����̃f�[�^
	SET_BOM,				// �{����z�u
	DETH,					// ���S
	LOST,					// �l�b�g���[�N�ؒf���ɐ���(�z�X�g�͎����̃l�b�g���[�N�ɃZ�b�g����)
	MAX
};

struct MesHeader
{
	MesType type;
	unsigned char next = 0;	// �f�[�^���M�������̏ꍇ�Ɏ�������ꍇ0:�Ȃ�1
	unsigned short sendID;	// �������M���̃i���o�����O
	int length_;		// �������ǂ����ɂ�����炸�A�P��p�P�b�g�̃f�[�^��(int�̐�)
};

union unionData
{
	char cdata[4];	// ���̃}�X��[0]:�c�̃}�X��[1]:���C���[��[2]:���U�[�u��[3]
	int idata;
};

union Header
{
	MesHeader mesdata_;
	int data_[2];
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
	virtual bool RevUpdate();
	virtual bool SendUpdate();
	ActiveState ConnectHost(IPDATA hostIP);
	void SendMessageData();
	void ReservMessageData();
	void ClearDataPacket();
	void ClearRevPacket();
	void ClearSendPacket();
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
	ListInt GetNetWorkHandle()
	{
		return netHandle;
	}

	void SetNetWorkState(ActiveState activeState)
	{
		this->active_ = activeState;
	}

	void SetNetHandle(int handle)
	{
		netHandle.emplace_back(std::make_pair(handle,0));
	}

	void SetTMXData(std::shared_ptr<File::TMX_File> tmxdata)
	{
		tmxFile_ = tmxdata;
	}

	std::shared_ptr<File::TMX_File> GetTMxData()
	{
		return std::move(tmxFile_);
	}

	void SetDataPacket(int data)
	{
		dataPacket_.emplace_back(data);
	}
	
	MesPacket GetDataPacket()
	{
		return dataPacket_;
	}


	void SetRevPacket(int data)
	{
		revPacket_.emplace_back(data);
	}

	MesPacket GetRevPacket()
	{
		return revPacket_;
	}



	void SetSendPacket(int data)
	{
		sendPacket_.emplace_back(data);
	}


	MesPacket GetSendPacket()
	{
		return sendPacket_;
	}

protected:

	// �ϐ��錾
	const int portNum_ = 8086;
	ActiveState active_;
	ListInt netHandle;		// dxlib���g�p����n���h��

	InputState input_;
	ActiveFunc activeFunc_;
	
	int fileSize_;
	RevBox revdata_;
	MesHeader mesData_;
	unionData uniondata_;

	MesPacket dataPacket_;
	
	MesPacket revPacket_;
	MesPacket sendPacket_;

	int playerID_;
	int playerMax_;

	std::unordered_map< MesType, std::function<void(void)>> updateMesType_;
	std::unique_ptr<Timer> timer_;
	std::shared_ptr<File::TMX_File> tmxFile_ = nullptr;

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

