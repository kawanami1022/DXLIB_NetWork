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
	mesData_.data[0] = static_cast<int>(std::filesystem::file_size("map.tmx"));
	std::cout << "File Size:" << mesData_.data[0] << std::endl;
}

ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{
	return active_;
}

void NetWorkState::SendMessageData()
{

	auto num = 0;
	std::string lineData_;
	std::ifstream File("map.tmx", std::ios::binary);
	if (!File.is_open())
	{
		std::cout << "ファイルが開けませんでした" << std::endl;
		return;
	}

	std::cout << "これからデータを送信します" << std::endl;
;
	mesData_.type = MesType::TMX_SIZE;
	auto fileSize = std::filesystem::file_size("map.tmx");
	mesData_.data[0] = fileSize;
	std::cout << "送信するファイルサイズ" << mesData_.data[0] << std::endl;
	NetWorkSend(netHandle, &mesData_, sizeof(mesData_));

	mesData_.type = MesType::INIT;
	mesData_.data[0] = 0;


	while (!File.eof())
	{
		std::getline(File, lineData_);
		for (auto str:lineData_)
		{
			mesData_.data[1] <<= 8;
			mesData_.data[1] = str;
			mesData_.data[0]++;
			std::cout << num << ":" << static_cast<char>(mesData_.data[1]);

			if (mesData_.data[1] == '\r')
			{
				mesData_.data[1] <<= 8;
				mesData_.data[1] += 0x0A;
				mesData_.data[0]++;
			}
			if ((mesData_.data[0] % 4) == 0)
			{
				NetWorkSend(netHandle, &mesData_, sizeof(mesData_));
			}

			if (mesData_.data[0] >= fileSize) { break; }
		}
		if (mesData_.data[0] >= fileSize) { break; }
		std::cout << std::endl;
	}
	std::cout <<"計測時間:"<< timer_->IntervalMesurement() << std::endl;
}

void NetWorkState::ReservMessageData()
{
	std::string lineData_;

	timer_->StartMesurement();

	mesData_.type = MesType::TMX_SIZE;
	NetWorkRecv(netHandle, &mesData_, sizeof(mesData_));
	std::cout << "fileSize:" << mesData_.data[0] << std::endl;
	revdata_.resize(mesData_.data[0]);

	mesData_.type = MesType::INIT;

	// ネットワークバッファのデータサイズが0になったら終了
	while (GetNetWorkDataLength(netHandle) >= 0)
	{
		NetWorkRecv(netHandle, &mesData_, sizeof(mesData_));

		for (int idx = 0; idx < 4; idx++)
		{
			auto revData = mesData_.data[0] & 0x00ffffff;
			revData >>= 8 * 3;
			mesData_.data[0] <<= 8 * 1;
			revdata_.push_back(static_cast<char>(revData));
			std::cout << "[" << revdata_.back() << "]";
		}
		if(revdata_.back() =='\n')	std::cout << std::endl;
	}
	std::cout << "取得したデータサイズ:" <<"          "<< sizeof(revdata_)<< "byte"<<std::endl;
	std::cout << "計測時間:" << timer_->IntervalMesurement() << std::endl;
}
