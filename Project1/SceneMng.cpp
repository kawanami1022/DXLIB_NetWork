#include <iostream>
#include <DxLib.h>
#include "SceneMng.h"
#include "Scene/BaseScene.h"
#include "Scene/LoginScene.h"

SceneMng* SceneMng::hInstance = nullptr;

int SceneMng::Run()
{

	SetDoubleStartValidFlag(TRUE);
	ChangeWindowMode(true);
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	std::cout << "DXLib������" << std::endl;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	nowScene = std::make_unique<LoginScene>();

	// �`����ʂ𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);
	

	// ���[�v
	while (ProcessMessage() == 0)
	{
		nowScene = nowScene->input(std::move(nowScene));
		nowScene = nowScene->UpDate(std::move(nowScene));
		ScenManagerDraw();
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;
}

void SceneMng::ScenManagerDraw()
{
	ClsDrawScreen();
	nowScene->Draw();
	ScreenFlip();

}

SceneMng::SceneMng()
{
	SetChainCount_ = 0;
}

SceneMng::~SceneMng()
{

}



