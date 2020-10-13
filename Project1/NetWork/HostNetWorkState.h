#pragma once
#include "NetWorkState.h"
class HostNetWorkState :
	public NetWorkState
{
public:
	HostNetWorkState();
	~HostNetWorkState();
	NetWorkMode GetNetWorkMode();
};

