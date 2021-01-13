#pragma once
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include "../NetWork/NetWork.h"
#include "../Lib/Vector2/Vector2.h"

namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

class BaseScene;
using UniqueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();

	virtual UniqueBase input(UniqueBase) = 0;
	virtual UniqueBase UpDate(UniqueBase) = 0;
	virtual void Draw() = 0;
	void DrawOwnScreen(int screenID);
	void DrawOwnScreen();
protected:
	int screenSrcID_=0;
	int screen_;
	bool isInstance_ = true;
	// “ü—ÍŒn
	std::array<char, UCHAR_MAX> keyIdNow_;
	std::array<char, UCHAR_MAX> keyIdOld_;

	// ‰æ‘œ
	std::shared_ptr< File::TMX_File> tmxFile_;
	std::ofstream log;

private:
};



