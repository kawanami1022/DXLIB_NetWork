#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <thread>
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

enum class MesType:char
{
	STANBY,				// 初期化情報送信完了
	GAME_START,			// ホストから初期化情報での初期化完了、ゲーム開始(
	TMX_SIZE,
	TMX_DATA,
	POS
};

struct MesHeader
{
	MesType type;
	unsigned short sdata;
	unsigned char cdata = 0;
	uint32_t idata[2];
};

//struct MesHeader
//{
//	MesType type;
//	unsigned short sdata;
//	unsigned char cdata = 0;
//	unsigned int length;
//};


union unionData
{
	char cData[8];
	int iData[2];
	long long lData;
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
	Non,		// 未設定
	Wait,		// 接続待機状態(ホスト用)
	Init,		// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	Stanby,		// 初期化情報送信済みの開始待ち
	Play,		// ゲーム中(ホスト/ゲスト兼用)
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

	//ゲッターセッター
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

	// 変数宣言
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle = 0;		// dxlibが使用するハンドル
	KeyInput controller_;

	InputState input_;
	ActiveFunc activeFunc_;
	
	int fileSize_;
	RevBox revdata_;
	MesHeader mesData_;
	unionData uniondata_;


	std::unordered_map< MesType, std::function<void(void)>> updateMesType_;
	std::unique_ptr<Timer> timer_;
	std::shared_ptr<File::TMX_File> tmxFile_ = nullptr;

	// 関数
	virtual void UpdateFuncNon();	
	virtual void UpdateFuncWait();	// 接続待機状態(ホスト用)
	virtual void UpdateFuncInit();	// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	virtual void UpdateFuncStanby();	// 初期化情報送信済みの開始待ち
	virtual void UpdateFuncPlay();	// ゲーム中(ホスト/ゲスト兼用)
	virtual void UpdateFuncOFFLINE();

	void STANBY();
	void GAME_START();
	void TMX_SIZE();
};

