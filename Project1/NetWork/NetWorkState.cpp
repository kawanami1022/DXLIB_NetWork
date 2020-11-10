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
#include "../Lib/File/File.h"
#include "../Lib/strmanip/strmanip.h"
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
	mesData_.sendID = 0;
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
	unsigned int intSendCount = 0;
	std::string linestring;
	unsigned int dataSize=0;
	unsigned int sendDataLength = 0;
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

	// 送信データ長を求める
	File::GetLineString(1, &linestring, "ini/setting.txt");		// 調査用文字列を取得する
	dataSize = std::stoi(strmanip::ExtractTheStrDblQt(linestring, "byte length"))/sizeof(int);
	std::cout << "dataSize:		" << dataSize << std::endl;


	//dataPacketの添え字[0]:TMXSIZE	[1]:TMXDATAを送る
	Header headerdata{ MesType::TMX_SIZE,0,0,1 };
	dataPacket.insert(dataPacket.begin(), headerdata.data_[0]);
	headerdata.mesdata_ = { MesType::TMX_DATA,0,0,dataPacket.size()-1 };
	dataPacket.insert(dataPacket.begin() + 1, headerdata.data_[0]);
	dataPacket.insert(dataPacket.begin() + 2, headerdata.data_[1]);

	for (auto DATAPACKET : dataPacket)
	{
		std::cout << std::hex << DATAPACKET << std::endl;
	}


	std::cout << "これからデータを送信します" << std::endl;
	timer_->StartMesurement();

	int sendLength = 0;
	do {
		// 送信用のデータの長さを求める
		sendLength = dataSize - MESHEADER_INT < dataPacket.size() ? 
			dataSize - MESHEADER_INT : dataPacket.size() - MESHEADER_INT;
		
		// 次のデータが存在するのか確かめる
		headerdata.mesdata_.next = dataPacket.size() > dataSize ? 1 : 0;
		dataPacket[1] = headerdata.data_[0];

		// int型のマップデータ格納変数が0になるまで処理する
		auto flag = NetWorkSend(netHandle, dataPacket.data(), sizeof(MesPacket) * (sendLength + MESHEADER_INT));
		dataPacket.erase(dataPacket.begin() + MESHEADER_INT, dataPacket.begin() + MESHEADER_INT + sendLength);
		headerdata.mesdata_.sendID++;

	} while (dataPacket.size() > MESHEADER_INT);



	std::cout << "計測時間:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
	std::cout << std::endl;



	mapId.clear();
	active_ = ActiveState::Play;
	// debug display

}

void NetWorkState::ReservMessageData()
{
	
	std::string lineData_;
	Header headerdata{MesType::STANBY};
	auto id = 0; 
	unsigned int dataSize = 0;


	if(GetNetWorkDataLength(netHandle)<=0)
	{
		std::cout << "headerデータが読み込めませんでした!" << std::endl;
		return;
	}
	
	MesPacket tmpPacketData;
	do {
		int recvdata = 0;
		NetWorkRecv(netHandle, &recvdata, sizeof(int));
		headerdata.data_[0] = recvdata;
		NetWorkRecv(netHandle, &recvdata, sizeof(int));
		headerdata.data_[0] = recvdata;
		if (headerdata.mesdata_.type == MesType::TMX_DATA)
		{	NetWorkRecv(netHandle, &recvdata, sizeof(int));
			headerdata.data_[1] = recvdata;
			dataPacket.resize(dataSize += headerdata.mesdata_.length_);}


		NetWorkRecv(netHandle, tmpPacketData.data(), headerdata.mesdata_.length_ * sizeof(int));
		dataPacket.insert(dataPacket.end(), tmpPacketData.begin(), tmpPacketData.end());

	} while (headerdata.mesdata_.next == 1);
	
	std::cout << "データを受け取りました" << std::endl;

	for (auto DATAPACKET : dataPacket)
	{
		std::cout << std::hex << DATAPACKET << std::endl;
	}
	active_ = ActiveState::Play;
}
