#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
TitleScene::TitleScene():BaseScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{

	GetDrawScreenSize(&screen_size_x, &screen_size_y);

	IpNetWork;

	// 関数初期化
	mode_ = UpdateMode::SetNetWork;
	updateFunc_ = { { UpdateMode::SetNetWork,std::bind(&TitleScene::SetNetWork,this) },
					{ UpdateMode::SetHostIP,std::bind(&TitleScene::SetHostIP,this) },
					{ UpdateMode::StartInit,std::bind(&TitleScene::StartInit,this) },
					{ UpdateMode::Play,std::bind(&TitleScene::Play,this) } };

	DrawFunc_ = { { UpdateMode::SetNetWork,std::bind(&TitleScene::SetNetWorkDraw,this) },
					{ UpdateMode::SetHostIP,std::bind(&TitleScene::SetHostIPDraw,this) },
					{ UpdateMode::StartInit,std::bind(&TitleScene::StartInitDraw,this) },
					{ UpdateMode::Play,std::bind(&TitleScene::PlayDraw,this) } };
}

UniqueBase TitleScene::input(UniqueBase nowScene)
{

	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	IpNetWork->Update();
	updateFunc_[mode_]();
	return nowScene;
}

void TitleScene::Draw()
{
	ClsDrawScreen();
	DrawFunc_[mode_]();
	ScreenFlip();
}

void TitleScene::SetNetWork()
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

	std::cout << "状態は" << static_cast<int>(IpNetWork->GetActice()) << "です\n" << std::endl;

}

void TitleScene::SetHostIP()
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
	if (activeState_== ActiveState::Stanby)
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

void TitleScene::StartInit()
{
	std::cout << "---------StartInit-----------" << std::endl;

	// 画像読み込み
	Handle = LoadGraph("Image/PURPLE_Puyo.png");
	std::cout << Handle << std::endl;
	mode_ = UpdateMode::Play;
}

void TitleScene::Play()
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

}

void TitleScene::SetNetWorkDraw()
{
}

void TitleScene::SetHostIPDraw()
{
	
}

void TitleScene::StartInitDraw()
{
}

void TitleScene::PlayDraw()
{
	DrawFormatString(0, 0, 0xffffff, "InputMode.move_way:%5d", IpNetWork->GetInputState());
	DrawGraph(pos_x, pos_y, Handle, true);
}
