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

	bool SetBomb(int ownerID, int selfID, Vector2 pos, bool sendNet);	// ownerID 親のID selfID 自身のID pos 爆弾を置く位置  sendNet
	//void DrawOwnScreen();
	GameScene();
	~GameScene();

private:
	// 変数名
	std::shared_ptr<Map> map_;
	std::vector<std::shared_ptr<Character>> character_;
	bool Init();

	std::unordered_map< NetWorkMode, std::function<void()>> updateNetWorkModeFunc_;
};

