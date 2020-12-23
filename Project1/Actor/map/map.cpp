#include <iostream>
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
	if (tmxFile_->tiledMap_["map"].titleID_[actorPos.x][actorPos.y] == 8)
	{
		return MAP_ID::BLOCK_BREAK;
	}

	if (tmxFile_->tiledMap_["map"].titleID_[actorPos.x][actorPos.y] == 6)
	{
		return MAP_ID::BLOCK_INBREAK;
	}
	if (tmxFile_->tiledMap_["map"].titleID_[actorPos.x][actorPos.y] == 7)
	{
		return MAP_ID::BLOCK_INBREAK;
	}
	return MAP_ID::NON;
}

int Map::GetGridID(Position2 GridPos, std::string Layer)
{
	if (GridPos.x < tmxFile_->width_ && GridPos.y < tmxFile_->height_)
	{
		return tmxFile_->tiledMap_[Layer].titleID_[GridPos.x][GridPos.y];
	}
	// ‚»‚±‚Étile‚ª‘¶Ý‚µ‚È‚¢
	return -1;
}

Vector2 Map::GetMapSize()
{
	return Vector2(tmxFile_->width_,tmxFile_->height_);
}

//
Position2 Map::GetTilePos(Position2 actorPos)
{
	return actorPos / TileSize * TileSize;
}

std::vector<std::shared_ptr<Character>> Map::SponePlayer()
{
	Position2 sponePos;
	Position2 Tile = Position2(0, 0);
	std::vector<std::shared_ptr<Character>> characterTmp;
	std::cout << "‚±‚ê‚©‚çcharacterƒ’¶¬‚·‚é" << std::endl;
	for (int x=0;x< tmxFile_->width_;x++)
	{
		for (int y = 0; y < tmxFile_->height_; y++)
		{
			Tile = { x,y };
			if (SponePlayerFlag(Tile))
			{
				characterTmp.emplace_back(std::make_shared<Character>(Position2(Tile * 32)));
				std::cout << characterTmp.size() << std::endl;
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
	if (GetMapId(actorPos) == MAP_ID::BLOCK_INBREAK)
	{
		return true;
	}
	return false;
}

