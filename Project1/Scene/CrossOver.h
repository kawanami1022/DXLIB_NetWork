#pragma once
#include "Transion.h"
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
	void DrawOwnScreen();
};

