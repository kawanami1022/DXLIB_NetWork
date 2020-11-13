#pragma once
#include <memory>
#include "../Actor.h"

namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

enum TILED_ID
{
	
};


class Map:public Actor
{
public:
	Map();
	~Map();
	void Draw();


private:
	std::shared_ptr< File::TMX_File> tmxFile_;
	std::unique_ptr<int[]> tileHandle_;
};

