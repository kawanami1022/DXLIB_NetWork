#pragma once
#include <string>
#include <map>
#include <memory>
#include "../Lib/Vector2/Vector2.h"
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
	virtual void DrawOwnScreen() = 0;
protected:
	int screenSrcID_;
	// “ü—ÍŒn
	// ‰æ‘œ
private:
};



