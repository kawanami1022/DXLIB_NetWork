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

bool NetWorkState::RevUpdate()
{
	if (GetLostNetWork() != -1)
	{
		revPacket_.clear();
		std::cout << "�ڑ����؂�Ă܂�!" << std::endl;
		return false;
	}
	// �l�b�g���[�N�o�b�t�@�ɗ��܂��Ă���f�[�^�����݂��邩���ɂ񂷂�
	while (GetNetWorkDataLength(netHandle) > 0)
	{
		for (auto REVDATA : revPacket_)
		{
			NetWorkRecv(netHandle, &REVDATA, sizeof(int));
			
		}
	}

	for (auto REVDATA : revPacket_)
	{
		std::cout << "�󂯎�����f�[�^:" << std::hex << REVDATA << std::endl;
	}

	return true;
}

bool NetWorkState::SendUpdate()
{
	
	if (GetLostNetWork() != -1)
	{
		return false;
	}
	NetWorkSend(netHandle, sendPacket_.data(), sendPacket_.size() * sizeof(int));

	for (auto DATAPACKET : sendPacket_)
	{
		std::cout << "�������f�[�^:" << std::hex << DATAPACKET << std::endl;
	}
	std::cout << std::endl;
	sendPacket_.clear();
	return true;
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
		std::cout << "tmxdata���ǂݍ��߂܂���" << std::endl;
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
		mapdata = 0;
		for (unsigned int i = 0; i < 8; i++)
		{
			std::cout << std::hex << mapdata << ":" << std::endl;
			mapdata |= mapId.front();
			mapId.erase(mapId.begin());
			if (i != (8 - 1))mapdata <<= 4;
			if (!(mapId.size() > 0))
			{
				break;
			}
		}

		std::cout << "���M�p�f�[�^:" << std::hex << mapdata << std::endl;
		dataPacket_.push_back(mapdata);
	}

	// ���M�f�[�^�������߂�
	File::GetLineString(1, &linestring, "ini/setting.txt");		// �����p��������擾����
	dataSize = std::stoi(strmanip::ExtractTheStrDblQt(linestring, "byte length"))/sizeof(int);
	std::cout << "dataSize:		" << dataSize << std::endl;


	//dataPacket�̓Y����[0]:TMXSIZE	[1]:TMXDATA�𑗂�
	Header headerdata{ MesType::TMX_SIZE,0,0,1 };
	dataPacket_.insert(dataPacket_.begin(), headerdata.data_[0]);
	headerdata.mesdata_ = { MesType::TMX_DATA,0,0,static_cast<int>(dataPacket_.size())-1 };
	dataPacket_.insert(dataPacket_.begin() + 1, headerdata.data_[0]);
	dataPacket_.insert(dataPacket_.begin() + 2, headerdata.data_[1]);

	for (auto DATAPACKET : dataPacket_)
	{
		std::cout << std::hex << DATAPACKET << std::endl;
	}


	std::cout << "���ꂩ��f�[�^�𑗐M���܂�" << std::endl;
	timer_->StartMesurement();

	int sendLength = 0;
	do {
		// ���M�p�̃f�[�^�̒��������߂�
		sendLength = (dataSize - MESHEADER_INT < dataPacket_.size()) ? 
			dataSize - MESHEADER_INT : dataPacket_.size() - MESHEADER_INT;
		
		// ���̃f�[�^�����݂���̂��m���߂�
		headerdata.mesdata_.next = dataPacket_.size() > dataSize ? 1 : 0;
		dataPacket_[1] = headerdata.data_[0];

		// int�^�̃}�b�v�f�[�^�i�[�ϐ���0�ɂȂ�܂ŏ�������
		auto flag = NetWorkSend(netHandle, dataPacket_.data(), sizeof(MesPacket) * (sendLength + MESHEADER_INT));
		dataPacket_.erase(dataPacket_.begin() + MESHEADER_INT, dataPacket_.begin() + MESHEADER_INT + sendLength);
		headerdata.mesdata_.sendID++;

	} while (dataPacket_.size() > MESHEADER_INT);


	std::cout << "�v������:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
	std::cout << std::endl;

	mapId.clear();
	dataPacket_.clear();
	active_ = ActiveState::Play;
	// debug display

}

void NetWorkState::ReservMessageData()
{
	std::vector<int> mapId;	// tmxFile's tiledmap 
	std::string lineData_;
	Header headerdata{MesType::STANBY};
	auto id = 0; 
	unsigned int dataSize = 0;


	if(GetNetWorkDataLength(netHandle)<=0)
	{
		std::cout << "header�f�[�^���ǂݍ��߂܂���ł���!" << std::endl;
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
		{
			NetWorkRecv(netHandle, &headerdata.mesdata_.length_, sizeof(int));
			tmpPacketData.resize(headerdata.mesdata_.length_);
		}
		NetWorkRecv(netHandle, tmpPacketData.data(), headerdata.mesdata_.length_ * sizeof(int));
		dataPacket_.insert(dataPacket_.end(), tmpPacketData.begin(), tmpPacketData.end());

	} while (GetNetWorkDataLength(netHandle) <= 0 &&headerdata.mesdata_.next == 1);
	
	std::cout << "�f�[�^���󂯎��܂���" << std::endl;

	for (auto DATAPACKET : dataPacket_)
	{
		std::cout << std::hex << DATAPACKET << std::endl;
	}

	id = 0;
	for (auto DATAPACKET : dataPacket_)
	{
		for (int idx = 0; idx < 8; idx++)
		{
			id = (DATAPACKET & 0xf0000000) >> (4 * 7);
			mapId.push_back(id);
			DATAPACKET <<= 4;
		}
	}
	if (mapId.size() == 0)return;

	int idx = 0;
	for (auto Name : tmxFile_->name_)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			for (int x = 0; x < tmxFile_->width_; x++)
			{
				tmxFile_->tiledMap_[Name].titleID_[x][y] = mapId[x + y * tmxFile_->width_ + idx * tmxFile_->height_ * tmxFile_->width_];
			}
		}
		idx++;
	}

	for (auto Name : tmxFile_->name_)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			for (int x = 0; x < tmxFile_->width_; x++)
			{
				std::cout << tmxFile_->tiledMap_[Name].titleID_[x][y];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	dataPacket_.clear();
	active_ = ActiveState::Play;
}

void NetWorkState::ClearDataPacket()
{
	dataPacket_.clear();
}

void NetWorkState::ClearRevPacket()
{
	revPacket_.clear();
}

void NetWorkState::ClearSendPacket()
{
	sendPacket_.clear();
}


