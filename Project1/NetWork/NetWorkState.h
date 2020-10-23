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
protected:

	// 変数宣言
	const int portNum_ = 8086;
	ActiveState active_;
	int netHandle = 0;		// dxlibが使用するハンドル
	KeyInput controller_;

	InputState input_;
	ActiveFunc activeFunc_;


	// 関数
	virtual void UpdateFuncNon();	
	virtual void UpdateFuncWait();	// 接続待機状態(ホスト用)
	virtual void UpdateFuncInit();	// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	virtual void UpdateFuncStanby();	// 初期化情報送信済みの開始待ち
	virtual void UpdateFuncPlay();	// ゲーム中(ホスト/ゲスト兼用)
	virtual void UpdateFuncOFFLINE();

};

