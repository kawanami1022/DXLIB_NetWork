#pragma once
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"

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
	virtual NetWorkMode GetNetWorkMode();
	virtual bool Update();
	bool GetActive(void);
	bool ConnectHost(IPDATA hostIP);
	//virtual bool CheckNetWork() = 0;
protected:
	const int portNum_ = 8086;
	bool active_;
	int netHandle = 0;		// dxlibが使用するハンドル
	Pad controller_;
};

