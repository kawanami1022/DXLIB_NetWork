#include <chrono>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <ostream>
#include <iomanip>
#include <bitset>
#include <fstream>
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

	timer_ = std::make_unique<Timer>();
	mesData_.sendID = 0;
	for (auto& RESULT : result_)
	{
		RESULT = -1;
	}
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
	revPacket_.clear();
	if (GetLostNetWork() != -1)
	{
		std::cout << "�ڑ����؂�Ă܂�!" << std::endl;
		return false;
	}

	auto RevData = 0;
	// �l�b�g���[�N�o�b�t�@�ɗ��܂��Ă���f�[�^�����݂��邩���ɂ񂷂�
	for (auto NetHandle : netHandle)
	{
		while (GetNetWorkDataLength(NetHandle.first) > 0)
		{
			NetWorkRecv(NetHandle.first, &RevData, sizeof(int));
			revPacket_.emplace_back(RevData);
		}
	}

	return true;
}

bool NetWorkState::SendUpdate(std::pair<int, unsigned int> pairInt)
{

	if (GetLostNetWork() != -1)
	{
		return false;
	}

	NetWorkSend(pairInt.first, sendPacket_.data(), sendPacket_.size() * sizeof(int));

	sendPacket_.clear();
	return true;
}

// map�f�[�^�̑���M
void NetWorkState::CreateThreadMpdt(NetWorkMode mode)
{
	if (mode == NetWorkMode::GUEST)
	{
		std::thread reserveData(&NetWorkState::ReservMessageData, this);
		reserveData.detach();
	}

	if (mode == NetWorkMode::HOST)
	{
		std::thread sendData([&]() {NetWorkState::SendMessageData(netHandle); });
		sendData.detach();
	}
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

bool NetWorkState::SendMessageData(ListInt netHandle)
{
	std::cout << "-------------SendMessageData------------" << std::endl;
	std::vector<int> mapId;	// tmxFile's tiledmap 
	short sendNum = 0;
	unsigned int intSendCount = 0;
	std::string linestring;
	unsigned int dataSize=0;
	unsigned int sendDataLength = 0;
	std::vector<int> data;
	auto loadTmx = [&]() {
		if (tmxFile_ == nullptr)
		{
			std::cout << "tmxdata���ǂݍ��߂܂���" << std::endl;
			tmxFile_ = std::make_unique<File::TMX_File>();
			tmxFile_->load_TMX("map.tmx");
		}
	};

	Header headerdata{ MesType::STANBY_GUEST };



	// PlayerID�𑗐M���Ă݂�
	headerdata = { MesType::ID,0,0,2 };

	for (auto& NETHANDLE : netHandle)
	{
		data = { headerdata.data_[0],headerdata.data_[1],(int)(NETHANDLE.second),playerMax_ };
		if(NetWorkSend(NETHANDLE.first, data.data(), sizeof(int) * data.size()))
		{
			std::cout << "�ڑ�����!" << std::endl;
		}
		data.clear();
	}
	loadTmx();
	//TMX_SIZE,		{ MesType �w�b�_�[,�c�T�C�Y,���T�C�Y,���C���[��}
	unionData sizeData = { tmxFile_->height_ ,tmxFile_->width_,tmxFile_->nextlayerid_,0 };
	headerdata = {MesType::TMX_SIZE,0,0,1};
	data = { headerdata.data_[0],headerdata.data_[1], sizeData.idata };
	for (auto& NETHANDLE : netHandle)
	{
		if (NetWorkSend(NETHANDLE.first, data.data(), sizeof(int) * data.size()) == 0)
		{
			std::cout << "�ڑ�����!" << std::endl;
		}
	}
	data.clear();
	loadTmx();

	// ���M�pmapdata�쐬
	//	{MesType �w�b�_�[,�f�[�^}
	// mapdata�}��
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
			mapdata |= mapId.front()<<4*i;
			mapId.erase(mapId.begin());
			//if (i != (8 - 1))mapdata <<= 4;
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
	// ���̑��M�Ɏg����f�[�^�������߂�
	dataSize = std::stoi(strmanip::ExtractTheStrDblQt(linestring, "byte length"));
	std::cout << "dataSize:		" << dataSize << std::endl;
	auto idx = 0;
	do {
		headerdata.mesdata_ = { MesType::TMX_DATA,
										0,		// 1:���̃f�[�^������
							static_cast<unsigned short>(idx),
							static_cast<int>(dataPacket_.size())};
		dataPacket_.insert(dataPacket_.begin(), headerdata.data_[0]);
		dataPacket_.insert(dataPacket_.begin()+1, headerdata.data_[1]);

		std::cout << "---------------�f�[�^�𑗐M���܂�---------------" << std::endl;
		for (auto& NETHANDLE : netHandle)
		{
			if (NetWorkSend(NETHANDLE.first, dataPacket_.data(), dataPacket_.size() * sizeof(int)))
			{
				std::cout << "�ڑ�����!" << std::endl;
			}
		}
		dataPacket_.clear();
	} while (dataPacket_.size() > 0);

	headerdata.mesdata_ = { MesType::STANBY_HOST,0,0,0 };
	dataPacket_.emplace_back(headerdata.data_[0]);
	dataPacket_.emplace_back(headerdata.data_[1]);
	for (auto& NETHANDLE : netHandle)
	{
		if (NetWorkSend(NETHANDLE.first, dataPacket_.data(), dataPacket_.size() * sizeof(int)))
		{
			std::cout << "�ڑ�����!" << std::endl;
		}
	}

	return true;
}

bool NetWorkState::ReservMessageData()
{

	std::vector<int> mapId;	// tmxFile's tiledmap 
	std::string lineData_;
	Header headerdata{MesType::STANBY_GUEST};
	auto id = 0; 
	int revData=0;
	unsigned int dataSize = 0;
	MesPacket tmpPacketData;		///MesPacket:	std::vector<int>;
	std::ofstream ofs("RevMsgDt.txt", std::ios::out);

	// STANBY_HOST��������I��
	do {
		// netHandle�����݂���̂�
		if (netHandle.size() <= 0)continue;
		if (GetNetWorkDataLength(netHandle.front().first) == 0)continue;

#if 1
		NetWorkRecv(netHandle.front().first, &headerdata.data_[0], sizeof(int));
		if (headerdata.mesdata_.type == MesType::COUNT_DOWN_ROOM)
		{
			std::cout << "-------------COUNT_DOWN_ROOM-------------" << std::endl;
			NetWorkRecv(netHandle.front().first, &headerdata.start_, sizeof(std::chrono::system_clock::time_point));
			std::cout << "����:" << std::chrono::system_clock::to_time_t(headerdata.start_) << std::endl;
		}
		if (headerdata.mesdata_.type == (MesType::ID))
		{
			std::cout << "-------------TMX_ID-------------" << std::endl;
			NetWorkRecv(netHandle.front().first, &playerMax_, sizeof(int));
			NetWorkRecv(netHandle.front().first, &playerID_, sizeof(int));
			NetWorkRecv(netHandle.front().first, &playerMax_, sizeof(int));
			std::cout << "playerMax:" <<std::setw(5)<< playerMax_ << std::endl;
			std::cout << "playerID:" <<std::setw(5)<< playerID_ << std::endl;
		}

		if (headerdata.mesdata_.type == (MesType::TMX_SIZE))
		{
			std::cout << "-------------TMX_SIZE-------------" << std::endl;
			NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
			//�c�T�C�Y,���T�C�Y,���C���[��
			NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
				
			//���T�C�Y,�c�T�C�Y,���C���[��
			unionData uniondata{ 0,0,0,0 };
			uniondata.idata = revData;
			std::cout << "���T�C�Y"  << static_cast<int>(uniondata.cdata[0]) <<
				"  �c�T�C�Y:" << static_cast<int>(uniondata.cdata[1]) << std::endl;
			std::cout << "���C���[��" << static_cast<int>(uniondata.cdata[2])<<std::endl;

			tmxFile_->nextlayerid_ = uniondata.cdata[2];
			for (auto TILED_MAP : tmxFile_->tiledMap_)
			{
				TILED_MAP.second.height_ = uniondata.cdata[1];
				TILED_MAP.second.width_ = uniondata.cdata[0];
				TILED_MAP.second.titleData_.resize(uniondata.cdata[0] * uniondata.cdata[1]);
				for (unsigned int idx = 0; idx < TILED_MAP.second.width_; idx++)
				{
					TILED_MAP.second.titleID_.emplace_back(&TILED_MAP.second.titleData_[idx * TILED_MAP.second.height_]);
				}
			}
		}


		if (headerdata.mesdata_.type == (MesType::TMX_DATA))
		{
			std::cout << "-------------TMX_DATA-------------" << std::endl;

			// tmpPacket�̐����擾
			NetWorkRecv(netHandle.front().first, &revData, sizeof(int));	
			auto pdSize = tmpPacketData.size();
			tmpPacketData.reserve(pdSize +revData);
			tmpPacketData.resize(pdSize + revData);
			for(unsigned int idx= pdSize;idx<tmpPacketData.size();idx++)
			{
				NetWorkRecv(netHandle.front().first, &tmpPacketData[idx], sizeof(int));
				std::cout << std::hex << tmpPacketData[idx] << std::endl;
			}
		}

		if (headerdata.mesdata_.type == (MesType::STANBY_HOST))
		{
			std::cout << "-------------STANBY_HOST-------------" << std::endl;
			int id;
			for (auto TILED_MAP : tmpPacketData)
			{
				std::cout << std::hex << TILED_MAP << std::endl;
			}
			id = 0;
			for (auto TILED_MAP : tmpPacketData)
			{
				for (int idx = 0; idx < 8; idx++)
				{
					id = (TILED_MAP & 0x0000000f);
					mapId.push_back(id);
					TILED_MAP >>= 4;
				}
			}
			std::cout << std::endl;
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
			Header sendHeader{ MesType::STANBY_GUEST,0,0,0 };
			NetWorkSend(netHandle.front().first,&sendHeader, sizeof(Header));
			std::cout << "---------STANBY GUEST���M���܂���--------------" << std::endl;
		}
#elif	0
		NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
		if (!ofs) {
			std::cout << "���͎��s!" << std::endl;
		}
		std::cout << revData << std::endl;
		ofs << std::hex << revData << std::endl;
#endif

	} while (headerdata.mesdata_.type != MesType::STANBY_HOST);
	ofs.close();

	tmxFile_->DisplayTiledMap();
	active_ = ActiveState::Play;
	std::cout << "play" << std::endl;
	return true;
}

void NetWorkState::ClearDataPacket()
{
	dataPacket_.clear();
}

void NetWorkState::ClearRevPacket()
{
	for (auto NetHandle : netHandle)
	{
		NetWorkRecvBufferClear(NetHandle.first);
	}
	revPacket_.clear();
}

void NetWorkState::ClearSendPacket()
{
	sendPacket_.clear();
	sendPacket_.shrink_to_fit();
}


