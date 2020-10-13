#pragma once
#include <string>
#include <map>
#include <memory>

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
protected:
	// “ü—ÍŒn
	// ‰æ‘œ
private:
};



