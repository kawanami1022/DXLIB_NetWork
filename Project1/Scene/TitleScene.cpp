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
	// �摜�ǂݍ���
	Handle = LoadGraph("Image/PURPLE_Puyo.png");
	std::cout << Handle << std::endl;
	GetDrawScreenSize(&screen_size_x, &screen_size_y);



	auto ipData = IpNetWork->GetIP();
	std::cout <<(int)(ipData.d1) << '.' << (int)(ipData.d2) << '.' << (int)(ipData.d3) << '.' << (int)(ipData.d4) << std::endl;


	auto inputNum = 0;
	std::cout << "�������\�����Ă������� OFFLINE:0 HOST:1 GUEST:2" << std::endl;
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
		std::cout << "HOST�ɐݒ肳��Ă܂�" << std::endl;
		break;
	case NetWorkMode::GUEST:
		std::cout << "GUEST�ɐݒ肳��Ă܂�" << std::endl;
		std::cout << "IP�A�h���X����͂��Ă�������" << std::endl;
		std::cin >> ip;
		// ip�ɓ��͂��ꂽ����hostIp�ɓ����
		ssIp << ip;

		hostIp.d1 = GetIpNum();
		hostIp.d2 = GetIpNum();
		hostIp.d3 = GetIpNum();
		hostIp.d4 = GetIpNum();

		std::cout << "GEST��IP�A�h���X	:" << (unsigned int)(hostIp.d1) <<"." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) <<"�ɐݒ肳��܂���!"<< std::endl;
		succeedFlag = IpNetWork->ConnectHost(hostIp);
		if (succeedFlag)
		{
			std::cout << "�ڑ�����!" << std::endl;
		}
		else
		{
			std::cout << "�ڑ����s!" << std::endl;
		}
		break;
	case NetWorkMode::OFFLINE:
		std::cout << "OFFLINE�ɐݒ肳��Ă܂�" << std::endl;
		break;
	default:
		break;
	}
	std::cout << "��Ԃ�" << IpNetWork->GetActice() << "�ł�\n" << std::endl;


}

UniqueBase TitleScene::input(UniqueBase nowScene)
{
	//controller_.Update();
	//if (controller_.Hold(InputID::Left)) { pos_x -= 3; }
	//if (controller_.Hold(InputID::Right)) { pos_x += 3; }
	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	
	//IpNetWork->
	return nowScene;
}

void TitleScene::Draw()
{
	ClsDrawScreen();
	DrawGraph(pos_x, pos_y, Handle, true);
	ScreenFlip();
}
