#pragma once
#include "Transion.h"
constexpr int BRIGHT_MAX = 255;

class CrossOver :
    public Transion
{
public:
	CrossOver();
	CrossOver(UniqueBase, UniqueBase);
	~CrossOver();

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();

private:
	unsigned int brightParam_=0;
};

