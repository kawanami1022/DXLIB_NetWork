#include <DxLib.h>
#include "SceneMng.h"
#include "Scene/BaseScene.h"
#include "Scene/TitleScene.h"

SceneMng* SceneMng::hInstance = nullptr;

int SceneMng::Run()
{

	ChangeWindowMode(true);
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

	SetDrawScreen(DX_SCREEN_BACK);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	nowScene = std::make_unique<TitleScene>();

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	// ループ
	while (ProcessMessage() == 0)
	{
		nowScene = nowScene->input(std::move(nowScene));
		nowScene = nowScene->UpDate(std::move(nowScene));
		nowScene->Draw();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;
}

SceneMng::SceneMng()
{
	SetChainCount_ = 0;
	
}

SceneMng::~SceneMng()
{
}



