#pragma once
#include "BaseScene.h"
class GameScene :
	public BaseScene
{
public:
	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();
	//void DrawOwnScreen();
	GameScene();
	~GameScene();
private:
	bool Init();

};

