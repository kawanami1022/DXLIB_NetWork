#include <DxLib.h>
#include "../../NetWork/NetWork.h"
#include "../../Lib/File/TMX_File.h"
#include "../../Lib/Vector2/Vector2.h"
#include "../Character/Character.h"
#include "map.h"



Map::Map()
{
	tmxFile_ = IpNetWork->GetNetWorkState()->GetTMxData();
	tileHandle_ = std::make_unique<int[]>(12 + 1);
	tileHandle_[0] = -1;
	LoadDivGraph("Image/map.png", 12, 4, 3, tmxFile_->tileheight_, tmxFile_->tilewidth_, &tileHandle_[1]);
}



Map::~Map()
{
}

void Map::Draw()
{
	for (int y = 0; y < tmxFile_->height_; y++)
	{
		for (int x = 0; x < tmxFile_->width_; x++)
		{
			for (auto Name : tmxFile_->name_)
			{
				DrawExtendGraph(x * 32, y * 32, x * 32 + 32, y * 32 + 32, tileHandle_[tmxFile_->tiledMap_[Name].titleID_[x][y]], true);
			}
		}
	}
}

MAP_ID Map::GetMapId(Position2 actorPos)
{
	actorPos /= 32;
	if (tmxFile_->tiledMap_["map"].titleID_[actorPos.x][actorPos.y]==0)
	{
		return MAP_ID::NON;
	}
	return MAP_ID::BLOCK;
}

std::vector<std::shared_ptr<Character>> Map::SponePlayer()
{
	Position2 sponePos;
	Position2 Tile = Position2(0, 0);
	std::vector<std::shared_ptr<Character>> characterTmp;
	for (int x=0;x< tmxFile_->width_;x++)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			Tile = { x,y };
			if (SponePlayerFlag(Tile))
			{
				characterTmp.push_back(std::make_unique<Character>(Position2(Tile * 32)));
			}
		}
	}

	return characterTmp;
}

bool Map::SponePlayerFlag(Position2 tilePos)
{
	if (tmxFile_->tiledMap_["character"].titleID_[tilePos.x][tilePos.y] == 4)
	{
		return true;
	}
	return false;
}

bool Map::IsTurnRight(Position2 actorPos)
{
	if (GetMapId(actorPos) == MAP_ID::BLOCK)
	{
		return true;
	}
	return false;
}

