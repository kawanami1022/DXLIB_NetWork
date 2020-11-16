#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include "BaseScene.h"
class Map;
class Character;
enum class NetWorkMode;
using VecCharacter = std::vector<std::shared_ptr<Character>>;

class GameScene :
	public BaseScene
{
public:
	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();

	void UpdateHost();
	void UpdateGuest();
	void UpdateOFFLINE();
	//void DrawOwnScreen();
	GameScene();
	~GameScene();

private:
	// ïœêîñº
	std::unique_ptr<Map> map_;
	std::vector<std::shared_ptr<Character>> character_;
	bool Init();

	std::unordered_map< NetWorkMode, std::function<void()>> updateFunc_;
};

