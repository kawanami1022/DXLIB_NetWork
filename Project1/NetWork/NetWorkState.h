#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <thread>
#include <utility>
#include <chrono>
#include <DxLib.h>
#include "../input/controller.h"
#include "../input/Pad.h"
#include "../input/keyInput.h"

constexpr	auto MTU = 1454;
constexpr auto MESHEADER_INT = 3;
namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

class Timer;
enum class ActiveState;
struct MesHeader;
union unionData;
union Header;
enum class MesType :char;

using ActiveFunc = std::unordered_map< ActiveState, std::function<void(void)>>;
using RevBox = std::vector<char>;
using MesPacket = std::vector<int>;
using RevPacket = std::pair<MesType, std::vector<int>>;
using MesPacketList = std::vector<int>;
using ListInt = std::list<std::pair<int, unsigned int>>;		//int ネットハンドル:unsigned int :プレーヤーID

enum class MesType :char
{
	NON = 100,
	COUNT_DOWN_ROOM,				// 接続受付カウントダウン		{MesType ヘッダー,longlong 時間}
	ID,								// 自分のIDとプレーヤーの総数	{MesType ヘッダー,int 自分のID,int playerの総数}
	STANBY_HOST,				// 初期化情報送信完了(ゲスト用)		{MesType ヘッダー}		
	STANBY_GUEST,				// 初期化情報送信完了(ホスト用)		{MesType ヘッダー}
	COUNT_DOWN_GAME,			//全員の初期化完了ごのゲーム開始時間 {MesType ヘッダー,longlong 時間}
	TMX_SIZE,					//	{ MesType ヘッダー,縦サイズ,横サイズ,レイヤー数}
	TMX_DATA,					//	{MesType ヘッダー,データ}
	POS,						// ゲーム中のデータ		{MesType ヘッダー,ID,x,y,Dir}
	SET_BOM,				// ボムを配置	{MesType ヘッダー,設置したキャラのID,爆弾自体のID,x,y,int 型　爆発までの時間(3秒),long long型 爆弾設置時間,}
	DETH,					// 死亡	{MesType ヘッダー,自分のID}
	RESULT,				// 結果	{MesType ヘッダー,ID,ID,ID,ID,ID,}		
	LOST,					// ネットワーク切断時に生成(ホストは自分のネットワークにセットする)	{MesType ヘッダー}
	MAX
};

struct MesHeader
{
	MesType type;
	unsigned char next = 0;	// データ送信が分割の場合に次がある場合0:ない1
	unsigned short sendID;	// 分割送信時のナンバリング
	int length_;		// 分割かどうかにかかわらず、単一パケットのデータ長(intの数)
};

union unionData
{
	char cdata[4];	// 横のマス数[0]:縦のマス数[1]:レイヤー数[2]:リザーブ数[3]
	int idata;
};

union Header
{
	MesHeader mesdata_;
	int data_[2];
	std::chrono::system_clock::time_point start_;
};

struct InputState
{
	unsigned int moveDir;
};

enum class NetWorkMode
{
	NON,
	OFFLINE,
	HOST,
	GUEST,
	MAX
};

enum class ActiveState
{
	Non,		// 未設定
	Wait,		// 接続待機状態(ホスト用)
	Init,		// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	Stanby,		// 初期化情報送信済みの開始待ち
	Play,		// ゲーム中(ホスト/ゲスト兼用)
	OFFLINE
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual bool Update();
	virtual bool RevUpdate();
	virtual bool SendUpdate(std::pair<int, unsigned int>);
	virtual void CreateThreadMpdt(NetWorkMode mode);
	ActiveState ConnectHost(IPDATA hostIP);
	bool SendMessageData(ListInt netHandle);
	bool ReservMessageData();
	void ClearDataPacket();
	void ClearRevPacket();
	void ClearSendPacket();
	//ゲッターセッター
	virtual NetWorkMode GetNetWorkMode();


	InputState GetInputState()
	{
		return input_;
	}
	ActiveState GetActive(void);
	const int GetPortNum() {
		return portNum_;
	}
	ListInt GetNetWorkHandle()
	{
		return netHandle;
	}

	void SetNetWorkState(ActiveState activeState)
	{
		this->active_ = activeState;
	}

	void SetNetHandle(int handle)
	{
		netHandle.emplace_back(std::make_pair(handle,playerID_));
		playerID_++;
	}

	void SetTMXData(std::shared_ptr<File::TMX_File> tmxdata)
	{
		tmxFile_ = tmxdata;
	}

	std::shared_ptr<File::TMX_File> GetTMxData()
	{
		return std::move(tmxFile_);
	}

	void SetDataPacket(int data)
	{
		dataPacket_.emplace_back(data);
	}
	
	MesPacket GetDataPacket()
	{
		return dataPacket_;
	}


	void SetRevPacket(int data)
	{
		revPacket_.emplace_back(data);
	}
	
	void SetResult(int id)
	{
		for (auto& RESULT : result_)
		{
			if (RESULT == -1)
			{
				RESULT = id;
				break;
			}
		}
	}

	std::array<int, 5> GetResult()
	{
		return result_;
	}

	MesPacket GetRevPacket()
	{
		return revPacket_;
	}

	void SetSendPacket(int data)
	{
		sendPacket_.emplace_back(data);
	}
	
	MesPacket GetSendPacket()
	{
		return sendPacket_;
	}

	// Get playerID_.
	int GetPlID()
	{
		return playerID_;
	}

	int GetPIMax()
	{
		return playerMax_;
	}

	std::chrono::system_clock::time_point GetStartTime()
	{
		return startTime_;
	}

	bool GetisSetSendStart()
	{
		return isSetSendStart_;
	}

protected:

	// 変数宣言
	const int portNum_ = 8086;
	ActiveState active_;
	ListInt netHandle;		// dxlibが使用するハンドル

	InputState input_;
	ActiveFunc activeFunc_;
	
	int fileSize_;
	RevBox revdata_;
	MesHeader mesData_;
	unionData uniondata_;

	MesPacket dataPacket_;
	
	MesPacket revPacket_;
	MesPacket sendPacket_;

	std::array<int,5> result_;

	int playerID_;
	int playerMax_=1;

	long long time_;
	bool isSetSendStart_;		// true: 開始時刻生成済み
	std::chrono::system_clock::time_point roomTime_;
	std::chrono::system_clock::time_point startTime_;
	
	std::unordered_map< MesType, std::function<void(void)>> updateMesType_;
	std::unique_ptr<Timer> timer_;
	std::shared_ptr<File::TMX_File> tmxFile_ = nullptr;
	
	// 関数
	virtual void UpdateFuncNon();	
	virtual void UpdateFuncWait();	// 接続待機状態(ホスト用)
	virtual void UpdateFuncInit();	// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	virtual void UpdateFuncStanby();	// 初期化情報送信済みの開始待ち
	virtual void UpdateFuncPlay();	// ゲーム中(ホスト/ゲスト兼用)
	virtual void UpdateFuncOFFLINE();

	void STANBY();
	void GAME_START();
	void TMX_SIZE();
};

