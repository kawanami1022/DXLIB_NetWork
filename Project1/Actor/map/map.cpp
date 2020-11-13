#include <DxLib.h>
#include "../../NetWork/NetWork.h"
#include "map.h"
#include "../../Lib/File/TMX_File.h"



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
