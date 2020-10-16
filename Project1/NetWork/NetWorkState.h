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

	//ゲッターセッター
	virtual NetWorkMode GetNetWorkMode();
	InputState GetInputState()
	{
		return input_;
	}
	bool GetActive(void);
protected:
	const int portNum_ = 8086;
	bool active_;
	int netHandle = 0;		// dxlibが使用するハンドル
	KeyInput controller_;


	// 変数宣言
	InputState input_;
};

