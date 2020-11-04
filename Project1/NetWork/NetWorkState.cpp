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

	std::cout << "これからデータを送信します" << std::endl;
	timer_->StartMesurement();
	mesData_.type = MesType::TMX_SIZE;
	mesData_.idata[0] = static_cast<int>(mapId.size());
	NetWorkSend(netHandle, &mesData_, sizeof(MesHeader));
	auto sendId = 0;

	auto idx = 0;
	while (idx <= mapId.size())
	{

		std::memset(&mesData_.idata, 0, sizeof(MesHeader));
		mesData_.type = MesType::TMX_DATA;
		for (int id = 0; id < 16; id++)
		{
			if (idx >= mapId.size())
			{
				std::cout << std::hex << mesData_.idata[0] << std::setw(8) << std::hex << mesData_.idata[1] << std::setw(8) << ":" << mesData_.sdata << std::endl;
				NetWorkSend(netHandle, &mesData_, sizeof(MesHeader));
				LogMesData.push_back(mesData_);
				mesData_.sdata++;
				break;
			}
			uniondata_.lData |= mapId[idx];
			if (id < 16 - 1)uniondata_.lData <<= 4;
			idx++;
		}
		if (idx >= mapId.size()) { break; }
		mesData_.idata[0] = uniondata_.iData[0];
		mesData_.idata[1] = uniondata_.iData[1];

		logUnion.lData = uniondata_.lData;
		for (int i = 0; i < 16; i++)
		{
			auto id = logUnion.lData & 0xf000000000000000;
			logUnion.lData <<= 4;
			id >>= 15 * 4;
		}
		std::cout << std::hex << mesData_.idata[0] << std::setw(8) << std::hex << mesData_.idata[1] <<std::setw(8)<<":"<<std::dec<< mesData_.sdata << std::endl;
		NetWorkSend(netHandle, &mesData_, sizeof(MesHeader));
		mesData_.sdata++;
		LogMesData.push_back(mesData_);
	}
	std::cout << "計測時間:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
	std::cout << std::endl;

	for (const auto LOG_MES_DATA : LogMesData)
	{;
		if (LOG_MES_DATA.type == MesType::TMX_DATA)
		{
			logUnion.iData[0] = LOG_MES_DATA.idata[0];
			logUnion.iData[1] = LOG_MES_DATA.idata[1];
			for (int i = 0; i < 16; i++)
			{
				auto id = logUnion.lData & 0xf000000000000000;
				logUnion.lData <<= 4;
				id >>= 15 * 4;
				LogMapData.push_back(id);
			}
		}
	}

	for (int layer = 0; layer < tmxFile_->nextlayerid_ - 1; layer++)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			for (int x = 0; x < tmxFile_->width_; x++)
			{
				int id = x + y * tmxFile_->width_ + layer * (tmxFile_->width_) * (tmxFile_->height_);
				//std::cout << id;
				std::cout << std::hex << LogMapData[id];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
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
			uniondata_.iData[0] = mesData_.idata[0];
			uniondata_.iData[1] = mesData_.idata[1];
			for (int i = 0; i < 8; i++)
			{
				auto idx = uniondata_.cData[i];

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
