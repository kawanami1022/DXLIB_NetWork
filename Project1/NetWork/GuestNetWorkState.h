#pragma once
#include "NetWorkState.h"
class GuestNetWorkState :
	public NetWorkState
{
public:
	GuestNetWorkState();
	~GuestNetWorkState();
	NetWorkMode GetNetWorkMode();
	ActiveState ConnectHost(IPDATA hostIP);
	bool CheckNetWork();
	bool Update();
	//ゲッターセッター

private:

	// 関数
	void UpdateFuncWait();	// 接続待機状態(ホスト用)
	void UpdateFuncInit();	// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	void UpdateFuncStanby();	// 初期化情報送信済みの開始待ち
	void UpdateFuncPlay();	// ゲーム中(ホスト/ゲスト兼用)
	void UpdateFuncOFFLINE();
};

