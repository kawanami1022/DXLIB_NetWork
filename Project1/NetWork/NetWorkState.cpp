#include <fstream>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"
#include "../Lib/File/TMX_File.h"
#include "NetWorkState.h"

NetWorkState::NetWorkState()
{
	mesData_.type = MesType::TMX_SIZE;
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
	mesData_.data[0] = static_cast<int>(std::filesystem::file_size("map.tmx"));
	std::cout << "File Size:" << mesData_.data[0] << std::endl;
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
	mesData_.data[0] = static_cast<int>(std::filesystem::file_size("map.tmx"));
	std::cout << "File Size:" << mesData_.data[0] << std::endl;
}

ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{
	return active_;
}

void NetWorkState::SendMessageData()
{
	NetWorkSend(netHandle, &mesData_, sizeof(mesData_));
	std::string lineData_;

	std::fstream File("map.tmx");
	if (!File.is_open())
	{
		std::cout << "ファイルが開けませんでした" << std::endl;
	}
	while (!File.eof())
	{
		std::getline(File, lineData_);
		for (int idx = 0; idx < lineData_.length(); idx++)
		{
			NetWorkSend(netHandle ,&lineData_.data()[idx], sizeof(char));
			std::cout << "[" << lineData_.data()[idx] << "]";
		}
		std::cout << std::endl;
	}
}

void NetWorkState::ReservMessageData()
{
	NetWorkRecv(netHandle, &mesData_, sizeof(mesData_));
	std::cout << "fileSize:" << mesData_.data[0] << std::endl;
	std::string lineData_;
	char chrdata;

	
	
	NetWorkRecv(netHandle, &chrdata, sizeof(char));
	for (int idx = 0; idx < static_cast<int>(lineData_.length()), idx++;)
	{
		std::cout << "[" << lineData_.data()[idx] << "]";
		std::cout << std::endl;
	}
}
