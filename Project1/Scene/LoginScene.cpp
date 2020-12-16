#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <thread>
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../Lib/File/TMX_File.h"
#include "LoginScene.h"
#include "GameScene.h"
#include "CrossOver.h"
#include "../Time/Time.h"

LoginScene::LoginScene():BaseScene()
{
	Init();
}

LoginScene::~LoginScene()
{
	DeleteGraph(CirlcleHandle_);
	std::cout << "------------LoginScene終了----------------" << std::endl;
}

void LoginScene::Init()
{

	GetDrawScreenSize(&screen_size_x, &screen_size_y);

	IpNetWork;

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

	tmxFile_ = std::make_unique<File::TMX_File>();

}

UniqueBase LoginScene::input(UniqueBase nowScene)
{

	return nowScene;
}

UniqueBase LoginScene::UpDate(UniqueBase nowScene)
{

	IpNetWork->Update();
	
	updateFunc_[mode_](nowScene);

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
	//ClsDrawScreen();
	SetDrawScreen(screenSrcID_);
	DrawFormatString(0, 0, 0xffffff, displayMode[mode_].data());
	
	DrawFunc_[mode_]();
}


void LoginScene::SetNetWork(UniqueBase& scene)
{
	std::cout << "---------SetNetWork----------" << std::endl;
	auto ipData = IpNetWork->GetIP();
	std::cout << (int)(ipData.d1) << '.' << (int)(ipData.d2) << '.' << (int)(ipData.d3) << '.' << (int)(ipData.d4) << std::endl;


	auto inputNum = 0;
	std::cout << "文字列を表示してください OFFLINE:1 HOST:2 GUEST:3" << std::endl;
	std::cin >> inputNum;

	for (int mode = 0; mode < (int)(NetWorkMode::MAX); mode++)
	{
		if (inputNum == mode)
		{
			IpNetWork->SetNetWorkMode((NetWorkMode)(mode));
			break;
		}
		if (mode == (int)(NetWorkMode::MAX))
		{

		}
	}

	auto NetWorkMode = IpNetWork->GetNetWorkMode();

	if (NetWorkMode == NetWorkMode::HOST)
	{
		std::cout << "HOSTに設定されてます" << std::endl;
		mode_ = UpdateMode::StartInit;
	}
	else if (NetWorkMode == NetWorkMode::GUEST)
	{
		std::cout << "GUESTに設定されてます" << std::endl;
		IpNetWorkState->CreateThreadMpdt(NetWorkMode);
		mode_ = UpdateMode::SetHostIP;
	}
	else if (NetWorkMode == NetWorkMode::OFFLINE)
	{
		std::cout << "OFFLINEに設定されてます" << std::endl;
		mode_ = UpdateMode::StartInit;
	}
	if (!tmxFile_->load_TMX("map.tmx"))
	{
		std::cout << "読み取りに失敗!" << std::endl;
	}
	IpNetWorkState->SetTMXData(tmxFile_);
	std::cout << "状態は" << static_cast<int>(IpNetWork->GetActive()) << "です\n" << std::endl;

}

void LoginScene::SetHostIP(UniqueBase& scene)
{
	std::cout << "---------SetHostIP----------" << std::endl;
	IPDATA hostIp = { 0,0,0,0 };
	std::string ip, data; std::stringstream ssIp;
	ActiveState activeState_;
	auto GetIpNum = [&]()
	{
		std::getline(ssIp, data, '.');
		return atoi(data.c_str());
	};

	std::cout << "GUESTに設定されてます" << std::endl;
	std::cout << "IPアドレスを入力してください" << std::endl;
	std::cin >> ip;
	// ipに入力された情報をhostIpに入れる
	ssIp << ip;

	hostIp.d1 = GetIpNum();
	hostIp.d2 = GetIpNum();
	hostIp.d3 = GetIpNum();
	hostIp.d4 = GetIpNum();

	std::cout << "HOSTのIPアドレス	:" << (unsigned int)(hostIp.d1) << "." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) << "に設定されました!" << std::endl;
	activeState_ = IpNetWork->ConnectHost(hostIp);
	if (activeState_== ActiveState::Init){
		std::cout << "接続成功!" << std::endl;
		mode_ = UpdateMode::StartInit;
	}else{
		std::cout << "接続失敗!" << std::endl;
		mode_ = UpdateMode::SetHostIP;}
}

void LoginScene::StartInit(UniqueBase& scene)
{
	std::cout << "---------StartInit-----------" << std::endl;
	// 画像読み込み
	Handle = LoadGraph("Image/PURPLE_Puyo.png");

	std::cout << Handle << std::endl;

	mode_ = UpdateMode::Play;
}

void LoginScene::Play(UniqueBase& scene)
{
	if (IpNetWorkState->GetActive()==ActiveState::Play)
	{
		auto nextScene = std::make_unique<GameScene>();
		scene = std::make_unique<CrossOver>(std::move(scene), std::move(nextScene));
	}

	if (IpNetWorkState->GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (CheckHitKey(KEY_INPUT_SPACE) == 1)
		{
			IpNetWorkState->SetNetWorkState(ActiveState::Stanby);
			std::cout << "-----------StanbyMode----------" << std::endl;
		}
	}

	if (IpNetWorkState->GetNetWorkMode() ==NetWorkMode::OFFLINE)
	{
		auto nextScene = std::make_unique<GameScene>();
		scene = std::make_unique<CrossOver>(std::move(scene), std::move(nextScene));
	}

}

void LoginScene::SetNetWorkDraw()
{
}

void LoginScene::SetHostIPDraw()
{
	
}

void LoginScene::StartInitDraw()
{
}

void LoginScene::PlayDraw()
{

	DrawGraph(pos_x, pos_y, Handle, true);
	
}
