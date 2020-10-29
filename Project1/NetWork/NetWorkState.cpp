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
	std::vector<int> mapId;
	if (tmxFile_==nullptr)
	{
		std::cout << "tmxdataが読み込めません" << std::endl;
		return;
	}
	std::memset(&mesData_, 0, sizeof(MesData));
	mesData_.type = MesType::TMX_SIZE;
	mesData_.idata[0] = 200;
	NetWorkSend(netHandle, &mesData_, sizeof(MesData));


	std::cout << "これからデータを送信します" << std::endl;
	;	auto sendId = 0;
	for (auto LAYERNAME : tmxFile_->name_)
	{
		auto idx = 0;
		mesData_.type = MesType::INIT;
		while (idx < tmxFile_->tiledMap_[LAYERNAME].titleData_.size())
		{
			std::memset(&mesData_, 0, sizeof(MesData));
			uniondata_.lData = 0;
			mesData_.type = MesType::INIT;
			mesData_.sdata = sendId;
			for (int id = 0; id < 16; id++)
			{
				uniondata_.lData |= tmxFile_->tiledMap_[LAYERNAME].titleData_[idx++];

				if (id < 16 - 1)uniondata_.lData <<= 4;
				if (idx >= tmxFile_->tiledMap_[LAYERNAME].titleData_.size()) { break; }
			}
			std::cout << std::setw(15) << "longdata:"<<std::setfill('0') << std::right << std::setw(16) << std::hex << uniondata_.lData << std::endl;
			mesData_.idata[0] = uniondata_.iData[0];
			mesData_.idata[1] = uniondata_.iData[1];

			std::cout << std::setw(15) <<"送信用data:"<< std::setfill('0') << std::right << std::setw(8) << std::hex << mesData_.idata[1] <<
				std::setfill('0') << std::right << std::setw(8) << std::hex << mesData_.idata[0] << std::endl;
			NetWorkSend(netHandle, &mesData_, sizeof(MesData));
			sendId++;
		}
	}

	std::cout <<"計測時間:"<< timer_->IntervalMesurement() << std::endl;
}

void NetWorkState::ReservMessageData()
{
	std::string lineData_;
	std::vector<int> mapData;
	auto id = 0;
	while (GetNetWorkDataLength(netHandle) <= 0)
	{

		NetWorkRecv(netHandle, &mesData_, sizeof(MesData));
		if (mesData_.type == MesType::TMX_SIZE)
		{
			mapData.resize(tmxFile_->height_ * tmxFile_->width_ * (tmxFile_->nextlayerid_ - 1));
			id = 0;
			std::memset(&mapData, 0, sizeof(mapData));
		}


		if (mesData_.type == MesType::INIT)
		{
			uniondata_.iData[0] = mesData_.idata[0];
			uniondata_.iData[1] = mesData_.idata[1];
			for (int i = 0; i < 8; i++)
			{
				auto idx = uniondata_.cData[i];

				mapData[id] = idx & 0x0f;
				id++;
				mapData[id] = idx & 0xf0;
				id++;
			}
		}
	}
}
