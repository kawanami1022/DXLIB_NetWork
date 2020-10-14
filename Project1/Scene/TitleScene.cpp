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
	IPDATA hostIp;
	std::string ip; std::string chr; std::stringstream ssIp;
	switch (IpNetWork->GetNetWorkMode())
	{
	case NetWorkMode::HOST:
		std::cout << "HOST�ɐݒ肳��Ă܂�" << std::endl;
		break;
	case NetWorkMode::GUEST:
		std::cout << "GUEST�ɐݒ肳��Ă܂�" << std::endl;
			std::cin >> ip;
			
			hostIp=IPDATA{ 
				static_cast<unsigned char>(ip.data()[0]), 
				static_cast<unsigned char>(ip.data()[1]),
				static_cast<unsigned char>(ip.data()[2]),
				static_cast<unsigned char>(ip.data()[3]) };
		
			while (std::getline(ssIp, chr,',')) {     // �X�y�[�X�i' '�j�ŋ�؂��āC�i�[
			}

		IpNetWork->ConnectHost(hostIp);
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
	return nowScene;
}

UniqueBase TitleScene::UpDate(UniqueBase nowScene)
{
	return nowScene;
}

void TitleScene::Draw()
{
}
