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
	MesPacket ldataPacket;
	unionData packetUnion;

	// debug display's variables
	std::vector<MesHeader> LogMesData;
	std::vector<int> LogMapData;
	unionData logUnion;

	if (tmxFile_==nullptr)
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


	auto idx = 0;
	for (auto MAP_ID : mapId)
	{
		packetUnion.idata |= MAP_ID;

		if (((idx % 8 == 0) && idx != 0))
		{
			ldataPacket.push_back(packetUnion);
			packetUnion.idata = 0;
		}
		else if (idx >= mapId.size()-1)
		{
			// 左端にシフト演算
			std::cout << std::hex << packetUnion.idata << std::endl;
			packetUnion.idata <<= 4 * (8 - idx % 8);
			ldataPacket.push_back(packetUnion);
			std::cout << std::hex << packetUnion.idata<<std::endl;
		}
		else {
			packetUnion.idata <<= 4;
		}

		idx++;
	}

	std::cout << std::endl;

	for (auto LDATAPACKET : ldataPacket)
	{
		std::cout <<std::setw(8)<< std::hex << LDATAPACKET.idata << std::endl;
	}

	std::cout << std::endl;

	auto sendDataSize = ldataPacket.size() * sizeof(unionData) + sizeof(short) + sizeof(char) + sizeof(char);

	std::cout << "ldataPacket:" << sendDataSize << std::endl;
	idx = 0;

	std::cout << "これからデータを送信します" << std::endl;
	timer_->StartMesurement();

	auto SendData = [&]() {
		mesData_.type = MesType::TMX_SIZE;
		mesData_.sdata = sendDataSize;
		std::cout << "送信データ量:" << std::dec << sendDataSize << std::endl;
		NetWorkSend(netHandle, &mesData_, sizeof(MesHeader));
	};


	while (ldataPacket.size() >0)
	{
		auto packetSize = ldataPacket.size();
		std::memset(&mesData_, 0, sizeof(mesData_));
		sendDataSize = packetSize * sizeof(unionData);
		if (sendDataSize < MTU)
		{
			SendData();
		}
		else
		{
			sendDataSize = ((ldataPacket.size() % MTU - 1) * sizeof(unionData));
			SendData();
		}

		packetSize = sendDataSize / sizeof(unionData);
		mesData_.type = MesType::TMX_DATA;
		mesData_.length_.resize( ldataPacket.size());
		std::memset(&mesData_, 0, sizeof(mesData_));
		for (int ID = 0; ID < packetSize; ID++)
		{
			mesData_.length_[ID]=ldataPacket[ID];
		}
		for (int ID = 0; ID < packetSize; ID++)
		{
			ldataPacket.erase(ldataPacket.begin());
		}

		NetWorkSend(netHandle, &mesData_, sizeof(MesHeader));
		std::cout << "ldataPacket: " << std::dec << ldataPacket.size() << std::endl;
	}

	std::cout << "計測時間:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
	std::cout << std::endl;
	mapId.clear();
	
	// debug display
	//	int id = MESDATA_LENGTH.idata & 0xf0000000;
	for (auto Name : tmxFile_->name_)
	{
		for (int idx = 0; idx < tmxFile_->height_ * tmxFile_->width_; idx++)
		{
			tmxFile_->tiledMap_[Name].titleData_[idx];
		}
	}
	

}

void NetWorkState::ReservMessageData()
{
	std::string lineData_;
	std::vector<int> mapData;
	auto id = 0;
	while (GetNetWorkDataLength(netHandle) <= 0)
	{

		NetWorkRecv(netHandle, &mesData_, sizeof(MesHeader));
		if (mesData_.type == MesType::TMX_SIZE)
		{
			mapData.resize(tmxFile_->height_ * tmxFile_->width_ * (tmxFile_->nextlayerid_ - 1));
			id = 0;
			std::memset(&mapData, 0, sizeof(mapData));
		}


		if (mesData_.type == MesType::TMX_DATA)
		{
			//uniondata_.iData[0] = mesData_.length_[0];
			//uniondata_.iData[1] = mesData_.length_[1];
			for (int i = 0; i < 8; i++)
			{
				auto idx = uniondata_.cdata[i];

				mapData[id] = (idx & 0xf0) >> 4;
				id++;
				mapData[id] = idx & 0x0f;
				id++;
			}
		}
	}

	for (int layer = 0; layer < tmxFile_->nextlayerid_ - 1; layer++)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			for (int x = 0; x < tmxFile_->width_; x++)
			{
				int id = x + y * tmxFile_->width_ + layer * tmxFile_->width_ * tmxFile_->height_;
				std::cout << mapData[id];
				std::cout << ":" << id << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}


}
