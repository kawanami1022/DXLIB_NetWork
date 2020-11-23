#pragma once
#include "NetWorkState.h"

class TMX_File;

class HostNetWorkState :
	public NetWorkState
{
public:
	HostNetWorkState();
	~HostNetWorkState();
	NetWorkMode GetNetWorkMode();
	bool CheckNetWork();
	bool Update();


protected:
private:
	// 関数
	void UpdateFuncNon();
	void UpdateFuncWait();		// 接続待機状態(ホスト用)
	void UpdateFuncInit();		// 初期化中(ゲーム開始準備中、ホストゲスト兼用)
	void UpdateFuncStanby();	// 初期化情報送信済みの開始待ち
	void UpdateFuncPlay();		// ゲーム中(ホスト/ゲスト兼用)
	void UpdateFuncOFFLINE();
};

