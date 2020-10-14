#include <iostream>
#include <sstream>
#include <string>
#include <DxLib.h>
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
	IPDATA hostIp;
	std::string ip; std::string chr; std::stringstream ssIp;
	switch (IpNetWork->GetNetWorkMode())
	{
	case NetWorkMode::HOST:
		std::cout << "HOSTに設定されてます" << std::endl;
		break;
	case NetWorkMode::GUEST:
		std::cout << "GUESTに設定されてます" << std::endl;
			std::cin >> ip;
			
			hostIp=IPDATA{ 
				static_cast<unsigned char>(ip.data()[0]), 
				static_cast<unsigned char>(ip.data()[1]),
				static_cast<unsigned char>(ip.data()[2]),
				static_cast<unsigned char>(ip.data()[3]) };
		
			while (std::getline(ssIp, chr,',')) {     // スペース（' '）で区切って，格納
			}

		IpNetWork->ConnectHost(hostIp);
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
	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	return nowScene;
}

void TitleScene::Draw()
{
}
