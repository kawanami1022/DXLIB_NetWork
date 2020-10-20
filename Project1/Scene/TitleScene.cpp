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

	// �֐�������
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
	std::cout << "�������\�����Ă������� OFFLINE:1 HOST:2 GUEST:3" << std::endl;
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
		std::cout << "HOST�ɐݒ肳��Ă܂�" << std::endl;
		mode_ = UpdateMode::StartInit;
	}
	else if (NetWorkMode == NetWorkMode::GUEST)
	{
		std::cout << "GUEST�ɐݒ肳��Ă܂�" << std::endl;
		mode_ = UpdateMode::SetHostIP;
	}
	else if (NetWorkMode == NetWorkMode::OFFLINE)
	{
		std::cout << "OFFLINE�ɐݒ肳��Ă܂�" << std::endl;
		mode_ = UpdateMode::StartInit;
	}

	std::cout << "��Ԃ�" << static_cast<int>(IpNetWork->GetActice()) << "�ł�\n" << std::endl;

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

	std::cout << "GUEST�ɐݒ肳��Ă܂�" << std::endl;
	std::cout << "IP�A�h���X����͂��Ă�������" << std::endl;
	std::cin >> ip;
	// ip�ɓ��͂��ꂽ����hostIp�ɓ����
	ssIp << ip;

	hostIp.d1 = GetIpNum();
	hostIp.d2 = GetIpNum();
	hostIp.d3 = GetIpNum();
	hostIp.d4 = GetIpNum();

	std::cout << "HOST��IP�A�h���X	:" << (unsigned int)(hostIp.d1) << "." << (unsigned int)(hostIp.d2) << "." << (unsigned int)(hostIp.d3) << "." << (unsigned int)(hostIp.d4) << "�ɐݒ肳��܂���!" << std::endl;
	activeState_ = IpNetWork->ConnectHost(hostIp);
	if (activeState_== ActiveState::Stanby)
	{
		std::cout << "�ڑ�����!" << std::endl;
		mode_ = UpdateMode::StartInit;
	}
	else
	{
		std::cout << "�ڑ����s!" << std::endl;
		mode_ = UpdateMode::SetHostIP;
	}
}

void TitleScene::StartInit()
{
	std::cout << "---------StartInit-----------" << std::endl;

	// �摜�ǂݍ���
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
