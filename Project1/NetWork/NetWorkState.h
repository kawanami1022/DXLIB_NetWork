#pragma once
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
};

