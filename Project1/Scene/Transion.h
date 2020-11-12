#pragma once
#include <memory>
#include "BaseScene.h"

using UniqueBase = std::unique_ptr<BaseScene>;

class Transion :
    public BaseScene
{
public:
	Transion();
	~Transion();

	virtual UniqueBase input(UniqueBase)=0;
	virtual UniqueBase UpDate(UniqueBase)=0;
	virtual void Draw()=0;
protected:
	UniqueBase oldScene_ = nullptr;
	UniqueBase nextScene_ = nullptr;
	
};

