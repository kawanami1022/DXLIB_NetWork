#include <DxLib.h>
#include "controller.h"
#include "keyInput.h"
#include "Pad.h"

int controller::joyPadNum_ = 0;


void controller::AutoChangeInput(SharCntr& sharCnt, int stageID)
{
	joyPadNum_ = GetJoypadNum();

	if (joyPadNum_ > stageID)
	{
		if (sharCnt->GetType() != ContType::Pad)
		{
			sharCnt = std::make_unique<Pad>();
			sharCnt->Setup(stageID);
		}
	}
	if (joyPadNum_ ==0)
	{
		if (sharCnt->GetType() != ContType::Key)
		{
			sharCnt = std::make_unique<KeyInput>();
			sharCnt->Setup(stageID);
		}
	}
}


