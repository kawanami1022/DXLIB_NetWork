#pragma once
#include "Transion.h"
constexpr auto INTERVAL_FLAME = 20;

enum class DRAW_STATE
{
	DRAW_STATE_OLD,
	DRAW_STATE_NOW
};



class OrgTrgScene :
    public Transion
{
public:
	OrgTrgScene();
	OrgTrgScene(UniqueBase, UniqueBase);
	~OrgTrgScene();

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();

private:
	unsigned int brightParam_=0;
	int intervalTime_;
	DRAW_STATE mode_;
};

