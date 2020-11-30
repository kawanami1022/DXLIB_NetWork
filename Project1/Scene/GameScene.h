#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include "BaseScene.h"
#include "../Actor/Item/Bomb/Bomb.h"

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

	bool SetBomb(int ownerID, int selfID, Vector2 pos, bool sendNet);	// ownerID �e��ID selfID ���g��ID pos ���e��u���ʒu  sendNet
	//void DrawOwnScreen();
	GameScene();
	~GameScene();

private:
	int flame = 0;
	// �ϐ���
	std::shared_ptr<Map> map_;
	CharacterList character_;
	bool Init();
	Bomb bomb_;
	std::unordered_map< NetWorkMode, std::function<void()>> updateNetWorkModeFunc_;
};

