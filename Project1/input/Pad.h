#pragma once
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <DxLib.h>
#include "controller.h"
struct Pad :public controller
{
	ContType GetType(void)override;
	void Update()override;
	bool push(InputID)override;
	bool release(InputID)override;
	bool Hold(InputID)override;
	bool separate(InputID)override;
	bool Setup(int)override;
	void changeInputTbl(int inputButton, InputID inputID)override;
	void PadForceFeedback(int, int)override;
	void DebugDrow(int id)override;

protected:
	std::array<XINPUT_STATE, 1> _xInputData;
	std::map<InputID, int> _xInputTbl;
	std::vector<int> PadIdList_;
private:
};

