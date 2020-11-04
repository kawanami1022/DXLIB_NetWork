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

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	std::cout << "DXLib初期化" << std::endl;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	nowScene = std::make_unique<LoginScene>();

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	

	// ループ
	while (ProcessMessage() == 0)
	{
		nowScene = nowScene->input(std::move(nowScene));
		nowScene = nowScene->UpDate(std::move(nowScene));
		ScenManagerDraw();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理
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



