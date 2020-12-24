#pragma once
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();
	UniqueBase input(UniqueBase nowScene);
	UniqueBase UpDate(UniqueBase nowScene);
	void Draw();

	void UpdateHost();
	void UpdateGuest();
	void UpdateOFFLINE();

	void Network();

};

