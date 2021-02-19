#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <thread>
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../Lib/File/TMX_File.h"
#include "../KeyBordInput/KeyBordInput.h"
#include "LoginScene.h"
#include "GameScene.h"
#include "CrossOver.h"
#include "OrgTrgScene.h"
#include "../Time/Time.h"

LoginScene::LoginScene():BaseScene()
{
	isInstance_ = true;
	Init();
}

LoginScene::~LoginScene()
{
	DeleteGraph(CirlcleHandle_);

	DeleteGraph(setNetWorkHandle_[0]);
	DeleteGraph(setNetWorkHandle_[1]);
	DeleteGraph(setNetWorkHandle_[2]);

	std::cout << "------------LoginScene終了----------------" << std::endl;
	isInstance_ = false;
}

void LoginScene::Init()
{

	GetDrawScreenSize(&screen_size_x, &screen_size_y);

	IpNetWork;
	keybord_ = std::make_unique<KeyBordInput>();

	// 関数初期化
	mode_ = UpdateMode::SetNetWork;
	updateFunc_ = { { UpdateMode::SetNetWork,std::bind(&LoginScene::SetNetWork,this,std::placeholders::_1) },
					{ UpdateMode::SetHostIP,std::bind(&LoginScene::SetHostIP,this,std::placeholders::_1) },
					{ UpdateMode::StartInit,std::bind(&LoginScene::StartInit,this,std::placeholders::_1) },
					{ UpdateMode::Play,std::bind(&LoginScene::Play,this,std::placeholders::_1) } };

	DrawFunc_ = { { UpdateMode::SetNetWork,std::bind(&LoginScene::SetNetWorkDraw,this) },
					{ UpdateMode::SetHostIP,std::bind(&LoginScene::SetHostIPDraw,this) },
					{ UpdateMode::StartInit,std::bind(&LoginScene::StartInitDraw,this) },
					{ UpdateMode::Play,std::bind(&LoginScene::PlayDraw,this) } };

	circlePos_ = Vector2(0, 0);
	CirlcleHandle_ = LoadGraph("Image/Circle.png", true);
	setNetWorkHandle_[0] = LoadGraph("Image/offline.png");
	setNetWorkHandle_[1] = LoadGraph("Image/Host.png");
	setNetWorkHandle_[2] = LoadGraph("Image/Guest.png");

	tmxFile_ = std::make_unique<File::TMX_File>();
	log.open("Login.txt");

}

UniqueBase LoginScene::input(UniqueBase nowScene)
{

	return nowScene;
}

UniqueBase LoginScene::UpDate(UniqueBase nowScene)
{
	std::memcpy(&keyIdNow_, &keyIdOld_, UCHAR_MAX);
	GetHitKeyStateAll(keyIdNow_.data());
	if (IpNetWork->GetNetWorkMode() == NetWorkMode::GUEST)
	{
		IpNetWork->Update();
	}
	
	updateFunc_[mode_](nowScene);
	SetDrawScreen(screenSrcID_);
	ClsDrawScreen();
	Draw();
	return nowScene;
}

void LoginScene::Draw()
{
	std::unordered_map<UpdateMode, std::string> displayMode = 
	{ {UpdateMode::SetNetWork,"SetNetWork"},
		{UpdateMode::SetHostIP,"SetHostIP"},
		{UpdateMode::Play,"Play"},
		{UpdateMode::StartInit,"StartInit"} };

	DrawFormatString(0, 0, 0xffffff, displayMode[mode_].data());
	
	DrawFunc_[mode_]();
}


void LoginScene::SetNetWork(UniqueBase& scene)
{
	
	auto inputNum = 0;
	auto netWorkMode = NetWorkMode::MAX;
	auto NetWorkUpdate = [&]() {
		IpNetWork->Update();
	};

	std::unordered_map<int, NetWorkMode> inputId_ =
	{ {KEY_INPUT_1,NetWorkMode::OFFLINE},
	{KEY_INPUT_2,NetWorkMode::HOST},
	{KEY_INPUT_3,NetWorkMode::GUEST}};
	// debug表示用変数
	for (auto InputID : inputId_)
	{
		if (CheckHitKey(InputID.first)==1)
		{
			IpNetWork->SetNetWorkMode(InputID.second);
			netWorkMode = IpNetWork->GetNetWorkMode();
			break;
		}
	}
	if (netWorkMode == NetWorkMode::HOST)
	{
		mode_ = UpdateMode::StartInit;

		std::thread update{ NetWorkUpdate };
		update.detach();
	}
	else if (netWorkMode == NetWorkMode::GUEST)
	{
		std::string line;

		//std::cout << "GUESTに設定されてます" << std::endl;
		IpNetWorkState->CreateThreadMpdt(netWorkMode);
		mode_ = UpdateMode::SetHostIP;
		std::ifstream file("ini/hostIP.txt", std::ios::in);
		while (!file.eof())
		{
			std::getline(file, line);
			ipAdress_.emplace_back(line);

		}
	}
	else if (netWorkMode == NetWorkMode::OFFLINE)
	{
		//std::cout << "OFFLINEに設定されてます" << std::endl;
		mode_ = UpdateMode::StartInit;
	}

	if (netWorkMode != NetWorkMode::MAX)
	{

		if (!tmxFile_->load_TMX("map.tmx"))
		{
			//std::cout << "読み取りに失敗!" << std::endl;
		}
		IpNetWorkState->SetTMXData(tmxFile_);
	}
	//std::cout << "状態は" << static_cast<int>(IpNetWork->GetActive()) << "です\n" << std::endl;

}

void LoginScene::SetHostIP(UniqueBase& scene)
{
	//std::cout << "---------SetHostIP----------" << std::endl;
	IPDATA hostIp = { 0,0,0,0 };
	std::string ip, data; std::stringstream ssIp;
	ActiveState activeState_;
	auto GetIpNum = [&]()
	{
		std::getline(ssIp, data, '.');
		return atoi(data.c_str());
	};

	keybord_->Update();
	if (keyIdNow_[KEY_INPUT_RETURN])
	{
		ip.assign(keybord_->GetInputString());
		ssIp << ip;

		hostIp.d1 = GetIpNum();
		hostIp.d2 = GetIpNum();
		hostIp.d3 = GetIpNum();
		hostIp.d4 = GetIpNum();

		std::cout << "HOSTのIPアドレス	:" << (unsigned int)(hostIp.d1) << "." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) << "に設定されました!" << std::endl;
		activeState_ = IpNetWork->ConnectHost(hostIp);


		if (activeState_ == ActiveState::Init) {
			bool isInputStr = true;
			std::cout << "接続成功!" << std::endl;
			mode_ = UpdateMode::StartInit;
			// Ipアドレス保存
			for (auto IP : ipAdress_)
			{
				// 重複文字列が存在するばあい
				if (IP == keybord_->GetInputString())
				{
					isInputStr = false;
				}
			}
			if (isInputStr)
			{
				std::fstream file("ini/hostIP.txt", std::ios::out|std::ios::app);
				file << (unsigned int)(hostIp.d1) << "." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) << "\n";
			}


		}
		else {
			std::cout << "接続失敗!" << std::endl;
			mode_ = UpdateMode::SetHostIP;
			keybord_->Reset();
		}
	}
}

void LoginScene::StartInit(UniqueBase& scene)
{
	auto mode = IpNetWorkState->GetNetWorkMode();

	auto now = std::chrono::system_clock::now();	// Hostの場合現在時刻を取得する
	//std::cout << "--------StartInit---------"<<std::endl;

	// 画像読み込み
	Handle = LoadGraph("Image/PURPLE_Puyo.png");

	if (mode == NetWorkMode::HOST)
	{
		if (IpNetWorkState->GetisSetSendStart())
		{
			//std::cout << "--------GetisSetSendStart---------" << std::endl;
			if (IpNetWorkState->GetStartTime() <= now)
			{
				std::cout << "--------CreateThreadMpdt---------" << std::endl;
				IpNetWorkState->CreateThreadMpdt(mode);
				mode_ = UpdateMode::Play;
			}
		}
		return;
	}
	else
	{
		mode_ = UpdateMode::Play;
	}
}

void LoginScene::Play(UniqueBase& scene)
{
	if (IpNetWorkState->GetActive()==ActiveState::Play)
	{
		std::thread network(&LoginScene::NetWork, this);
		network.detach();
		auto nextScene = std::make_unique<GameScene>();
		scene = std::make_unique<OrgTrgScene>(std::move(scene), std::move(nextScene));
	}

	if (IpNetWorkState->GetNetWorkMode() == NetWorkMode::HOST)
	{
			//IpNetWorkState->SetNetWorkState(ActiveState::Stanby);
			//std::cout << "-----------StanbyMode----------" << std::endl;
		auto nextScene = std::make_unique<GameScene>();
		scene = std::make_unique<OrgTrgScene>(std::move(scene), std::move(nextScene));
	}

	if (IpNetWorkState->GetNetWorkMode() ==NetWorkMode::OFFLINE)
	{
		auto nextScene = std::make_unique<GameScene>();
		scene = std::make_unique<OrgTrgScene>(std::move(scene), std::move(nextScene));
	}

}

void LoginScene::SetNetWorkDraw()
{
	DrawGraph(50, 50, setNetWorkHandle_[0], true);
	DrawGraph(300, 50, setNetWorkHandle_[1], true);
	DrawGraph(550, 50, setNetWorkHandle_[2], true);
}

void LoginScene::SetHostIPDraw()
{
	auto height_ = 50;
	for (auto IP : ipAdress_)
	{
		height_ += 16;
		DrawFormatString(0, height_, 0xffffff, "%s", IP.c_str());
	}
	keybord_->Draw();

}

void LoginScene::StartInitDraw()
{
}

void LoginScene::PlayDraw()
{
	
	DrawGraph(pos_x, pos_y, Handle, true);
	
}

void LoginScene::NetWork()
{
	std::cout << "network" << std::endl;
	while (isInstance_)
	{
		// Guest
		IpNetWorkState->RevUpdate();
		auto data = IpNetWorkState->GetRevPacket();
		if (log.is_open() == true)
		{
			for (auto DATA : data)
			{
				std::cout << DATA << std::endl;
				log << DATA << "\n";
			}
		}
	}
}
