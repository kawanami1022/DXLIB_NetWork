#pragma once
#include <DxLib.h>
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
	bool GetActive(void);
	bool ConnectHost(IPDATA hostIP);

protected:
	const int portNum_ = 8086;
	bool active_;
	int netHandle = 0;		// dxlibが使用するハンドル
};

