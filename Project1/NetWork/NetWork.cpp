#include <DxLib.h>
#include "NetWork.h"

NetWork* NetWork::hInstance = nullptr;

IPDATA NetWork::GetIP()
{
	IPDATA ipData;
	GetMyIPAddress(&ipData);
	return ipData;
}

NetWork::NetWork()
{
}
