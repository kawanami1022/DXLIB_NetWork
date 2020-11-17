#pragma once
#include <unordered_map>
#include <array>
#include <functional>

#include "BaseScene.h"

enum class UpdateMode;


class Timer;
using UpdateMapFunc = std::unordered_map<UpdateMode, std::function<void(UniqueBase&)>>;
using DrawMapFunc = std::unordered_map < UpdateMode, std::function<void(void)>>;
enum class UpdateMode
{
	SetNetWork,			// ホスト/ゲスト/オフラインの選択
	SetHostIP,			// ゲスト選択時のIPアドレス設定
	StartInit,			// スタート前の初期化
	Play,				// ゲーム中
	Max
};


class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();

	void Init();

private:

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();
	//void DrawOwnScreen();
	//updateFunc_
	void SetNetWork(UniqueBase& scene);
	void SetHostIP(UniqueBase& scene);
	void StartInit(UniqueBase& scene);
	void Play(UniqueBase& scene);
	//DrawFunc_
	void SetNetWorkDraw();
	void SetHostIPDraw();
	void StartInitDraw();
	void PlayDraw();

	// 変数
	UpdateMode mode_;
	UpdateMapFunc updateFunc_;
	DrawMapFunc DrawFunc_;
	
	int screen_size_x=800;
	int screen_size_y=600;

	int pos_x=300;
	int pos_y=300;
	Vector2Tmp<int> circlePos_;

	int Handle;
	int CirlcleHandle_ = 0;

};

