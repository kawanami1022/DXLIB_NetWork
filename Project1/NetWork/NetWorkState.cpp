#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <bitset>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"
#include "../Lib/File/TMX_File.h"
#include "../Time/Time.h"
#include "NetWorkState.h"


NetWorkState::NetWorkState()
{
	active_ = ActiveState::Non;
	input_.moveDir = 0;
	activeFunc_ = { 
				{ActiveState::Non,std::bind(&NetWorkState::UpdateFuncNon,this)},
				{ActiveState::Wait,std::bind(&NetWorkState::UpdateFuncWait,this)},
				{ActiveState::Init,std::bind(&NetWorkState::UpdateFuncInit,this)},
				{ActiveState::Stanby,std::bind(&NetWorkState::UpdateFuncStanby,this)},
				{ActiveState::Play,std::bind(&NetWorkState::UpdateFuncPlay,this)},
				{ActiveState::OFFLINE,std::bind(&NetWorkState::UpdateFuncOFFLINE,this)} };

	updateMesType_ =
	{ {MesType::TMX_SIZE ,std::bind(&NetWorkState::TMX_SIZE,this)},
		{MesType::GAME_START ,std::bind(&NetWorkState::GAME_START,this)},
		{MesType::STANBY ,std::bind(&NetWorkState::STANBY,this)} };
	timer_ = std::make_unique<Timer>();
	mesData_.sdata = 0;
}

NetWorkState::~NetWorkState()
{
}

NetWorkMode NetWorkState::GetNetWorkMode()
{
	return NetWorkMode::OFFLINE;
}

bool NetWorkState::Update()
{
	activeFunc_[active_]();
	return false;
}

ActiveState NetWorkState::GetActive(void)
{
	return active_;
}

void NetWorkState::UpdateFuncNon()
{
}

void NetWorkState::UpdateFuncWait()
{
}

void NetWorkState::UpdateFuncInit()
{
}

void NetWorkState::UpdateFuncStanby()
{
}

void NetWorkState::UpdateFuncPlay()
{
}

void NetWorkState::UpdateFuncOFFLINE()
{
}

void NetWorkState::STANBY()
{
}

void NetWorkState::GAME_START()
{
}

void NetWorkState::TMX_SIZE()
{
}

ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{
	return active_;
}

void NetWorkState::SendMessageData()
{
	std::vector<int> mapId;	// tmxFile's tiledmap 
	short sendNum = 0;
	// debug display's variables

	if (tmxFile_ == nullptr)
	{
		std::cout << "tmxdataが読み込めません" << std::endl;
		return;
	}
	std::memset(&mesData_, 0, sizeof(MesHeader));

	// substruction's mapId
	for (auto Name : tmxFile_->name_)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			for (int x = 0; x < tmxFile_->width_; x++)
			{
				mapId.push_back(tmxFile_->tiledMap_[Name].titleID_[x][y]);
			}
		}
	}


	auto mapdata = 0;
	//dataPacketの添え字[0]:TMXSIZE	[1]:TMXDATAを送る
	unionHeader headerdata{ MesType::TMX_SIZE,0,0,1 };
	dataPacket.push_back(headerdata.data_[0]);
	headerdata.mesdata_ = { MesType::TMX_DATA,0,0,dataPacket.size() * sizeof(int) };
	dataPacket.push_back(headerdata.data_[0]);

	while (mapId.size() > 0)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			mapdata |= mapId.front();
			mapId.erase(mapId.begin());
			if (i != (8 - 1))mapdata <<= 4;
			if (!(mapId.size() > 0))
			{
				break;
			}
		}
		dataPacket.push_back(mapdata);
	}


	std::cout << "これからデータを送信します" << std::endl;
	timer_->StartMesurement();

	// int型のマップデータ格納変数が0になるまで処理する
	NetWorkSend(netHandle, &dataPacket, sizeof(MesPacket)*dataPacket.size());

	std::cout << "計測時間:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
	std::cout << std::endl;
	mapId.clear();
	
	// debug display

}

void NetWorkState::ReservMessageData()
{
	std::string lineData_;
	std::vector<int> mapData;
	unionHeader headerdata{MesType::STANBY};
	auto id = 0;
	dataPacket.reserve((mapData.size() / 8 + 1 + 2));	//
	if (GetNetWorkDataLength(netHandle) > 0)
	{
		NetWorkRecv(netHandle, &dataPacket, sizeof(int) * dataPacket.size());
	}
	else
	{
		std::cout << "データが読み込めませんでした!" << std::endl;
		return;
	}

	
	if (dataPacket.size() > 2)
	{
		headerdata.data_[0] = dataPacket[1];
	}
	else
	{
		return;
	}

	if (headerdata.mesdata_.type == MesType::TMX_DATA)
	{
		dataPacket.erase(dataPacket.begin(), dataPacket.begin() + 1);
		mapData.resize(headerdata.mesdata_.length_ / sizeof(int));
	}

}
