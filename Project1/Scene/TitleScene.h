#pragma once
#include <array>
#include "BaseScene.h"

enum class UpdateMode
{
	SetNetWork,			// ホスト/ゲスト/オフラインの選択
	SetHostIP,			// ゲスト選択時のIPアドレス設定
	StartInit,			// スタート前の初期化
	Play,				// ゲーム中
	Max
};


class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Init();

private:

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();


	// 変数
	UpdateMode mode;
	typedef void (TitleScene::* UpdateFunc)();
	std::array<UpdateFunc, static_cast<int>(UpdateMode::Max)> drawFunc;


	int screen_size_x=800;
	int screen_size_y=600;


	int pos_x=300;
	int pos_y=300;
	int Handle;

};

