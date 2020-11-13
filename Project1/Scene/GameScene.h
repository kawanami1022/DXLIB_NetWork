#pragma once
#include <vector>
#include <memory>
#include "BaseScene.h"
class Map;
class Character;

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
	// ïœêîñº
	std::unique_ptr<Map> map_;
	std::vector<std::unique_ptr<Character>> character_;
	bool Init();
};

