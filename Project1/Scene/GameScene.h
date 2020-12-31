#pragma once
#include <mutex>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <chrono>
#include "BaseScene.h"
#include "../Actor/Item/Bomb/Bomb.h"
#include "../Actor/effect/explosion.h"

class Map;
class Character;

enum class NetWorkMode;
using CharacterList = std::vector<std::shared_ptr<Character>>;

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

	void Network();

	bool SetBomb(int ownerID, int selfID, Vector2 pos, bool sendNet);	// ownerID 親のID selfID 自身のID pos 爆弾を置く位置  sendNet
	bool SetFire(Position2 pos,int dst, Dir dir);
	GameScene();
	~GameScene();

private:
	int flame = 0;
	// 変数名
	std::shared_ptr<Map> map_;
	CharacterList character_;
	bool Init();
	std::unordered_map< NetWorkMode, std::function<void()>> updateNetWorkModeFunc_;
	std::chrono::system_clock::time_point start_;
	std::once_flag once;
	std::vector<std::unique_ptr<Bomb>> bomb_;
	std::vector<std::unique_ptr<explosion>> fire_;
	bool changeScene_=false;
};

