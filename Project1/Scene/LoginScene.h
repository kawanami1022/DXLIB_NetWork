#pragma once
#include <unordered_map>
#include <array>
#include <functional>
#include "BaseScene.h"

enum class UpdateMode;
class KeyBordInput;

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
	void SetNetWork(UniqueBase& scene);
	void SetHostIP(UniqueBase& scene);
	void StartInit(UniqueBase& scene);
	void Play(UniqueBase& scene);
	void SetNetWorkDraw();
	void SetHostIPDraw();
	void StartInitDraw();
	void PlayDraw();

	void NetWork();
	// 変数
	UpdateMode mode_;
	UpdateMapFunc updateFunc_;
	DrawMapFunc DrawFunc_;
	
	std::unique_ptr<KeyBordInput> keybord_;

	int screen_size_x=800;
	int screen_size_y=600;

	int pos_x=300;
	int pos_y=300;

	Vector2Tmp<int> circlePos_;

	int Handle;
	int CirlcleHandle_ = 0;
	std::array<int,3> setNetWorkHandle_;
	std::vector<std::string> ipAdress_;

};

