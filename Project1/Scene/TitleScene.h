#pragma once
#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Init();

private:

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();


	// ïœêî

	int screen_size_x=800;
	int screen_size_y=600;

	

};

