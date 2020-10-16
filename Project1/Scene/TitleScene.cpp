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
	// 画像読み込み
	Handle = LoadGraph("Image/PURPLE_Puyo.png");
	std::cout << Handle << std::endl;
	GetDrawScreenSize(&screen_size_x, &screen_size_y);



	auto ipData = IpNetWork->GetIP();
	std::cout <<(int)(ipData.d1) << '.' << (int)(ipData.d2) << '.' << (int)(ipData.d3) << '.' << (int)(ipData.d4) << std::endl;


	auto inputNum = 0;
	std::cout << "文字列を表示してください OFFLINE:0 HOST:1 GUEST:2" << std::endl;
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

	IPDATA hostIp = {0,0,0,0};
	std::string ip,data; std::stringstream ssIp;
	bool succeedFlag;
	auto GetIpNum = [&]()
	{
		std::getline(ssIp, data, '.');
		return atoi(data.c_str());
	};

	switch (IpNetWork->GetNetWorkMode())
	{
	case NetWorkMode::HOST:
		std::cout << "HOSTに設定されてます" << std::endl;
		break;
	case NetWorkMode::GUEST:
		std::cout << "GUESTに設定されてます" << std::endl;
		std::cout << "IPアドレスを入力してください" << std::endl;
		std::cin >> ip;
		// ipに入力された情報をhostIpに入れる
		ssIp << ip;

		hostIp.d1 = GetIpNum();
		hostIp.d2 = GetIpNum();
		hostIp.d3 = GetIpNum();
		hostIp.d4 = GetIpNum();

		std::cout << "HOSTのIPアドレス	:" << (unsigned int)(hostIp.d1) <<"." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) <<"に設定されました!"<< std::endl;
		succeedFlag = IpNetWork->ConnectHost(hostIp);
		if (succeedFlag)
		{
			std::cout << "接続成功!" << std::endl;
		}
		else
		{
			std::cout << "接続失敗!" << std::endl;
		}
		break;
	case NetWorkMode::OFFLINE:
		std::cout << "OFFLINEに設定されてます" << std::endl;
		break;
	default:
		break;
	}
	std::cout << "状態は" << IpNetWork->GetActice() << "です\n" << std::endl;


}

UniqueBase TitleScene::input(UniqueBase nowScene)
{

	auto InputMode = IpNetWork->GetInputState();
	if (InputMode.move_way & 0x02)
	{
		pos_x += 3;
	}
	if (InputMode.move_way & 0x08)
	{
		pos_x -= 3;
	}
	
	//controller_.Update();
	//if (controller_.Hold(InputID::Left)) { pos_x -= 3; }
	//if (controller_.Hold(InputID::Right)) { pos_x += 3; }
	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	
	IpNetWork->Update();
	return nowScene;
}

void TitleScene::Draw()
{
	ClsDrawScreen();
	DrawFormatString(0, 0, 0xffffff, "InputMode.move_way:%5d", IpNetWork->GetInputState());
	DrawGraph(pos_x, pos_y, Handle, true);
	ScreenFlip();
}
