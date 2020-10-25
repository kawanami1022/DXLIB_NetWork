#pragma once
#include <array>
#include "controller.h"

struct KeyInput:public controller
{
	ContType GetType(void)override;
	bool Setup(int no) override;	// ÉvÉåÅ[ÉÑÅ[ÇÃnumber
	void Update(void) override;
	bool push(InputID)override;
	bool Hold(InputID)override;
	bool release(InputID)override;
	bool separate(InputID)override;
	void changeInputTbl(int, InputID)override;
	void PadForceFeedback(int, int)override;
	void DebugDrow(int id)override;
private:
	std::array<char, 256> _keyData;
	std::map<InputID, int> _InputTbl;
};