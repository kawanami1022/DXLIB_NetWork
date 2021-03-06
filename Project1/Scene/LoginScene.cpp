#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../Lib/File/TMX_File.h"
#include "LoginScene.h"
#include "GameScene.h"
#include "CrossOver.h"
#include "../NetWork/NetWork.h"
#include "../Time/Time.h"

LoginScene::LoginScene():BaseScene()
{
	Init();
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init()
{

	GetDrawScreenSize(&screen_size_x, &screen_size_y);

	IpNetWork;

	// 関数初期化
	mode_ = UpdateMode::SetNetWork;
	updateFunc_ = { { UpdateMode::SetNetWork,std::bind(&LoginScene::SetNetWork,this) },
					{ UpdateMode::SetHostIP,std::bind(&LoginScene::SetHostIP,this) },
					{ UpdateMode::StartInit,std::bind(&LoginScene::StartInit,this) },
					{ UpdateMode::Play,std::bind(&LoginScene::Play,this) } };

	DrawFunc_ = { { UpdateMode::SetNetWork,std::bind(&LoginScene::SetNetWorkDraw,this) },
					{ UpdateMode::SetHostIP,std::bind(&LoginScene::SetHostIPDraw,this) },
					{ UpdateMode::StartInit,std::bind(&LoginScene::StartInitDraw,this) },
					{ UpdateMode::Play,std::bind(&LoginScene::PlayDraw,this) } };
	circlePos_ = Vector2(0, 0);
	CirlcleHandle_ = LoadGraph("Image/Circle.png", true);

}

UniqueBase LoginScene::input(UniqueBase nowScene)
{

	return nowScene;
}

UniqueBase LoginScene::UpDate(UniqueBase nowScene)
{

		
	IpNetWork->Update();
	updateFunc_[mode_]();
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		auto nextScene = std::make_unique<GameScene>();
		nowScene = std::make_unique<CrossOver>(std::move(nowScene), std::move(nextScene));
	}
	return nowScene;
}

void LoginScene::Draw()
{
	DrawOwnScreen();
	
}

void LoginScene::DrawOwnScreen()
{
	SetDrawScreen(screenSrcID_);
	ClsDrawScreen();
	DrawFunc_[mode_]();
}

void LoginScene::SetNetWork()
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
		mode_ = UpdateMode::SetHostIP;
	}
	else if (NetWorkMode == NetWorkMode::OFFLINE)
	{
		std::cout << "OFFLINEに設定されてます" << std::endl;
		mode_ = UpdateMode::StartInit;
	}

	std::cout << "状態は" << static_cast<int>(IpNetWork->GetActive()) << "です\n" << std::endl;

}

void LoginScene::SetHostIP()
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
	if (activeState_== ActiveState::Init)
	{
		std::cout << "接続成功!" << std::endl;
		
		mode_ = UpdateMode::StartInit;
		
	}
	else
	{
		std::cout << "接続失敗!" << std::endl;
		mode_ = UpdateMode::SetHostIP;
	}
}

void LoginScene::StartInit()
{
	std::cout << "---------StartInit-----------" << std::endl;
	
	// 画像読み込み
	Handle = LoadGraph("Image/PURPLE_Puyo.png");

	std::cout << Handle << std::endl;

	tmxFile_ = std::make_unique<File::TMX_File>();
	if (!tmxFile_->load_TMX("map.tmx"))
	{
		std::cout << "読み取りに失敗!" << std::endl;
	}
	IpNetWork->GetNetWorkState()->SetTMXData(tmxFile_);
	tileHandle_ = std::make_unique<int[]>(12+1);
	tileHandle_[0] = -1;
	LoadDivGraph("Image/map.png", 12, 4, 3, tmxFile_->tileheight_, tmxFile_->tilewidth_, &tileHandle_[1]);


	for (int idx = 0; idx < 13; idx++)
	{
		std::cout << tileHandle_[idx] << std::endl;
	}

	mode_ = UpdateMode::Play;
}

void LoginScene::Play()
{

	auto InputMode = IpNetWork->GetInputState();
	if (InputMode.moveDir & 0x02)
	{
		pos_x += 3;
	}
	if (InputMode.moveDir & 0x08)
	{
		pos_x -= 3;
	}

	if (circlePos_.y >= screen_size_y)
	{
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		std::uniform_int_distribution<int> distribution(0, screen_size_x);
		auto tmp_pos_x = distribution(engine);
		circlePos_.x = tmp_pos_x;
		circlePos_.y = 0;
	}
	circlePos_.y += 5;

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
	DrawFormatString(0, 0, 0xffffff, "InputMode.move_way:%5d", IpNetWork->GetInputState());


	DrawGraph(pos_x, pos_y, Handle, true);

	for (int y = 0; y < tmxFile_->height_; y++)
	{
		for (int x = 0; x < tmxFile_->width_; x++)
		{
			for (auto Name : tmxFile_->name_)
			{
				DrawExtendGraph(x * 32, y * 32, x * 32 + 32, y * 32 + 32, tileHandle_[tmxFile_->tiledMap_[Name].titleID_[x][y]], true);
			}
		}
	}

	DrawRotaGraph(circlePos_.x, circlePos_.y, 1, 0, CirlcleHandle_, true);
}
