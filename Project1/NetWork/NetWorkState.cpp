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
		
		std::cout << "接続が切れてます!" << std::endl;
		return false;
	}
	//if (GetNetWorkDataLength(netHandle) == 0) {
	//	std::cout << "データが受け取れてません!" << std::endl;
	//}
	auto RevData = 0;
	// ネットワークバッファに溜まっているデータが存在するかくにんする
	for (auto NetHandle : netHandle)
	{
		while (GetNetWorkDataLength(NetHandle.first) > 0)
		{
			NetWorkRecv(NetHandle.first, &RevData, sizeof(int));
			revPacket_.emplace_back(RevData);
		}
	}
	//for (auto REVDATA : revPacket_)
	//{
	//	std::cout << "受け取ったデータ:" << std::hex << REVDATA << std::endl;
	//}
	std::cout << std::endl;
	return true;
}

bool NetWorkState::SendUpdate(std::pair<int, unsigned int> pairInt)
{
	std::thread SendNetWorkThread([&]() {

		if (GetLostNetWork() != -1)
		{
			return false;
		}

		NetWorkSend(pairInt.first, sendPacket_.data(), sendPacket_.size() * sizeof(int));

		sendPacket_.clear();
		return true;
	});
	SendNetWorkThread.join();
	return true;
}

// mapデータの送受信
void NetWorkState::CreateThreadMpdt(NetWorkMode mode)
{
	if (mode == NetWorkMode::GUEST)
	{
		std::thread reserveData(&NetWorkState::ReservMessageData, this);
		reserveData.detach();
	}

	if (mode == NetWorkMode::HOST)
	{
		std::thread sendData([&]() {NetWorkState::SendMessageData(netHandle.back().first); });
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

bool NetWorkState::SendMessageData(int netHandle)
{
	std::vector<int> mapId;	// tmxFile's tiledmap 
	short sendNum = 0;
	unsigned int intSendCount = 0;
	std::string linestring;
	unsigned int dataSize=0;
	unsigned int sendDataLength = 0;
	// debug display's variables
	while (1)
	{

		if (tmxFile_ == nullptr)
		{
			std::cout << "tmxdataが読み込めません" << std::endl;
			tmxFile_->load_TMX("map.tmx");
			continue;
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
				//std::cout << std::hex << mapdata << ":" << std::endl;
				mapdata |= mapId.front();
				mapId.erase(mapId.begin());
				if (i != (8 - 1))mapdata <<= 4;
				if (!(mapId.size() > 0))
				{
					break;
				}
			}

			std::cout << "送信用データ:" << std::hex << mapdata << std::endl;
			dataPacket_.push_back(mapdata);
		}

		// 送信データ長を求める
		File::GetLineString(1, &linestring, "ini/setting.txt");		// 調査用文字列を取得する
		dataSize = std::stoi(strmanip::ExtractTheStrDblQt(linestring, "byte length"))/sizeof(int);
		std::cout << "dataSize:		" << dataSize << std::endl;

		//dataPacketの添え字[0]:TMXSIZE	[1]:TMXDATAを送る
		Header headerdata{ MesType::TMX_SIZE,0,0,1 };
		dataPacket_.insert(dataPacket_.begin(), headerdata.data_[0]);
		headerdata.mesdata_ = { MesType::TMX_DATA,0,0,static_cast<int>(dataPacket_.size()) - 1 };
		dataPacket_.insert(dataPacket_.begin() + 1, headerdata.data_[0]);
		dataPacket_.insert(dataPacket_.begin() + 2, headerdata.data_[1]);


		std::cout << "これからデータを送信します" << std::endl;
		timer_->StartMesurement();

		int sendLength = 0;
		do {
			// 送信用のデータの長さを求める
			sendLength = (dataSize - MESHEADER_INT < dataPacket_.size()) ?
				dataSize - MESHEADER_INT : dataPacket_.size() - MESHEADER_INT;

			// 次のデータが存在するのか確かめる
			headerdata.mesdata_.next = dataPacket_.size() > dataSize ? 1 : 0;
			dataPacket_[1] = headerdata.data_[0];

			// int型のマップデータ格納変数が0になるまで処理する
			auto flag = NetWorkSend(netHandle, dataPacket_.data(), sizeof(MesPacket) * (sendLength + MESHEADER_INT));
			dataPacket_.erase(dataPacket_.begin() + MESHEADER_INT, dataPacket_.begin() + MESHEADER_INT + sendLength);
			headerdata.mesdata_.sendID++;
		} while (dataPacket_.size() > MESHEADER_INT);
		dataPacket_.clear();


		std::cout << "計測時間:" << std::dec << timer_->IntervalMesurement().count() << std::endl;
		std::cout << std::endl;
	
		active_ = ActiveState::Stanby;
		break;
		// debug display
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

	std::ofstream ofs("RevMsgDt.txt",std::ios::out);
	// STANBY_HOSTが来たら終了
	do {
		// netHandleが存在するのか
		if (netHandle.size() <= 0)continue;
		if (GetNetWorkDataLength(netHandle.front().first) == 0)continue;

		NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
		headerdata.mesdata_.type = static_cast<MesType>(revData);
		ofs << std::hex << revData << "\n";

#ifdef DEBUG
		while (1)
		{
			if (netHandle.size() <= 0) {
				continue;
			}
			if (netHandle.size() <= 0 || GetNetWorkDataLength(netHandle.front().first) <= 0)
			{
				std::cout << "headerデータが読み込めませんでした!" << std::endl;
				continue;
			}

			MesPacket tmpPacketData;
			do {
				int recvdata = 0;
				NetWorkRecv(netHandle.front().first, &recvdata, sizeof(int));
				headerdata.data_[0] = recvdata;
				NetWorkRecv(netHandle.front().first, &recvdata, sizeof(int));
				headerdata.data_[0] = recvdata;
				if (headerdata.mesdata_.type == MesType::TMX_DATA)
				{
					NetWorkRecv(netHandle.front().first, &headerdata.mesdata_.length_, sizeof(int));
					tmpPacketData.resize(headerdata.mesdata_.length_);
				}
				NetWorkRecv(netHandle.front().first, tmpPacketData.data(), headerdata.mesdata_.length_ * sizeof(int));
				dataPacket_.insert(dataPacket_.end(), tmpPacketData.begin(), tmpPacketData.end());

			} while (GetNetWorkDataLength(netHandle.front().first) <= 0 && headerdata.mesdata_.next == 1);

			std::cout << "データを受け取りました" << std::endl;

			for (auto DATAPACKET : dataPacket_)
			{
				std::cout << std::hex << DATAPACKET << std::endl;
			}

			std::cout << "mapIdに挿入" << std::endl;

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

			active_ = ActiveState::Stanby;
			break;
		}

		std::cout << "データを受け取りました" << std::endl;

		for (auto DATAPACKET : dataPacket_)
		{
			std::cout << std::hex << DATAPACKET << std::endl;
		}

		std::cout << "mapIdに挿入" << std::endl;

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

		int idx = 0;
		//for (auto Name : tmxFile_->name_)
		//{
		//	for (int y = 0; y < tmxFile_->height_; y++)
		//	{
		//		for (int x = 0; x < tmxFile_->width_; x++)
		//		{
		//			tmxFile_->tiledMap_[Name].titleID_[x][y] = mapId[x + y * tmxFile_->width_ + idx * tmxFile_->height_ * tmxFile_->width_];
		//		}
		//	}
		//	idx++;
		//}
		//std::cout << "-------------COUNT_DOWN_ROOM-------------" << std::endl;
		//if (headerdata.mesdata_.type == MesType::COUNT_DOWN_ROOM)
		//{
		//	std::cout << "-------------COUNT_DOWN_ROOM-------------" << std::endl;
		//	NetWorkRecv(netHandle.front().first, &time_, sizeof(std::chrono::minutes));
		//	std::cout << "時間:" <<time_<< std::endl;
		//}
		//if (headerdata.mesdata_.type == (MesType::ID))
		//{
		//	std::cout << "-------------TMX_ID-------------" << std::endl;
		//	NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
		//	playerMax_ = revData;
		//	NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
		//	playerID_ = revData;
		//	std::cout << "playerMax:" <<std::setw(5)<< playerMax_ << std::endl;
		//	std::cout << "playerID:" <<std::setw(5)<< playerID_ << std::endl;
		//}

		//if (headerdata.mesdata_.type == (MesType::TMX_SIZE))
		//{
		//	std::vector<int> tmplist;
		//	std::cout << "-------------TMX_SIZE-------------" << std::endl;
		//	while (GetNetWorkDataLength(netHandle.front().first) > 0)
		//	{

		//		//縦サイズ,横サイズ,レイヤー数
		//		NetWorkRecv(netHandle.front().first, &revData, sizeof(int));
		//		tmplist.emplace_back(revData);

		//	}
		//	for (auto TMP_LIST : tmplist)
		//	{
		//		std::cout <<std::hex<< TMP_LIST << std::endl;
		//	}
			//縦サイズ,横サイズ,レイヤー数
			//auto height = revData;
			//auto width = revData;
			//auto layer = revData;
			//std::cout << "縦サイズ:" << height << "横サイズ:" << width << std::endl;
			//std::cout << "レイヤー数:" << layer << std::endl;

			//tmxFile_->nextlayerid_ = layer;
			//for (auto TILED_MAP : tmxFile_->tiledMap_)
			//{
			//	TILED_MAP.second.height_ = height;
			//	TILED_MAP.second.width_ = width;
			//	TILED_MAP.second.titleData_.resize(height * width);
			//	for (unsigned int idx = 0; idx < width; idx++)
			//	{
			//		TILED_MAP.second.titleID_.emplace_back(&TILED_MAP.second.titleData_[idx * height]);
			//	}
			//}
		//}

		//if (headerdata.mesdata_.type == (MesType::TMX_DATA))
		//{
		//	std::cout << "-------------TMX_DATA-------------" << std::endl;
		//		MesPacket tmpPacketData;
		//		do {
		//			int recvdata = 0;
		//			NetWorkRecv(netHandle.front().first, &recvdata, sizeof(int));
		//			tmpPacketData.emplace_back(recvdata);

		//		} while (GetNetWorkDataLength(netHandle.front().first) >0);
		//}

		//if (headerdata.mesdata_.type == (MesType::STANBY_HOST))
		//{
		//	std::cout << "-------------STANBY_HOST-------------" << std::endl;
		//}
#endif

	} while (headerdata.mesdata_.type != MesType::STANBY_HOST);
	std::cout << "-------------STANBY_HOST-------------" << std::endl;

#ifdef DEBUG
	while (1)
	{
		if (netHandle.size() <= 0) {
			continue;
		}
		if(netHandle.size()<=0||GetNetWorkDataLength(netHandle.front().first)<=0)
		{
			std::cout << "headerデータが読み込めませんでした!" << std::endl;
			continue;
		}
	
		MesPacket tmpPacketData;
		do {
			int recvdata = 0;
			NetWorkRecv(netHandle.front().first, &recvdata, sizeof(int));
			headerdata.data_[0] = recvdata;
			NetWorkRecv(netHandle.front().first, &recvdata, sizeof(int));
			headerdata.data_[0] = recvdata;
			if (headerdata.mesdata_.type == MesType::TMX_DATA)
			{
				NetWorkRecv(netHandle.front().first, &headerdata.mesdata_.length_, sizeof(int));
				tmpPacketData.resize(headerdata.mesdata_.length_);
			}
			NetWorkRecv(netHandle.front().first, tmpPacketData.data(), headerdata.mesdata_.length_ * sizeof(int));
			dataPacket_.insert(dataPacket_.end(), tmpPacketData.begin(), tmpPacketData.end());

		} while (GetNetWorkDataLength(netHandle.front().first) <= 0 &&headerdata.mesdata_.next == 1);
	
		std::cout << "データを受け取りました" << std::endl;

		for (auto DATAPACKET : dataPacket_)
		{
			std::cout << std::hex << DATAPACKET << std::endl;
		}

		std::cout << "mapIdに挿入" << std::endl;

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

		active_ = ActiveState::Stanby;
		break;
	}

	std::cout << "データを受け取りました" << std::endl;

	for (auto DATAPACKET : dataPacket_)
	{
		std::cout << std::hex << DATAPACKET << std::endl;
	}

	std::cout << "mapIdに挿入" << std::endl;

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

	int idx = 0;
	//for (auto Name : tmxFile_->name_)
	//{
	//	for (int y = 0; y < tmxFile_->height_; y++)
	//	{
	//		for (int x = 0; x < tmxFile_->width_; x++)
	//		{
	//			tmxFile_->tiledMap_[Name].titleID_[x][y] = mapId[x + y * tmxFile_->width_ + idx * tmxFile_->height_ * tmxFile_->width_];
	//		}
	//	}
	//	idx++;
	//}
#endif // DEBUG

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
}


