#pragma once
#include <vector>
#include <memory>
#include "../Actor.h"


constexpr int TileSize = 32;


namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

class Character;

enum class CHAR_ID
{
	PLAYER,
	NON,
};

enum class MAP_ID
{
	NON,
	BLOCK
};


class Map:public Actor
{
public:
	Map();
	~Map();
	void Draw();

	MAP_ID GetMapId(Position2 actorPos);
	int GetGridID(Position2 GridPos,std::string Layer);
	Vector2 GetMapSize();
	Position2 GetTilePos(Position2 actorPos);
	std::vector<std::shared_ptr<Character>> SponePlayer();

	bool SponePlayerFlag(Position2 tilePos);
	bool IsTurnRight(Position2 actorPos);
private:
	std::shared_ptr< File::TMX_File> tmxFile_;
	std::shared_ptr<int[]> tileHandle_;
};

