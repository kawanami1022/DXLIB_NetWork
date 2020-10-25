#pragma once
#include <memory>
#include <array>
#include <map>
#include "InputID.h"
#define STCI static_cast<int>

enum class Trg
{
	Now,
	Old,
	Max
};



//入力装置の種類
enum class ContType
{
	Key,
	Pad,
	Max
};

struct controller;

using SharCntr = std::shared_ptr<controller>;
using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID, TrgBool>;


struct controller
{
	void operator()()
	{
		Update();
	}

	CntData GetCntData(void)
	{
		return _data;
	}

	virtual ContType GetType(void) = 0;
	virtual bool Setup(int no) = 0;	// プレーヤーのnumber
	virtual bool push(InputID) = 0;
	virtual bool release(InputID) = 0;
	virtual bool Hold(InputID) = 0;
	virtual bool separate(InputID)=0;
	virtual void DebugDrow(int id) = 0;
	virtual void changeInputTbl(int, InputID)=0;
	virtual void PadForceFeedback(int, int)=0;
	void AutoChangeInput(SharCntr&, int);
protected:
	CntData _data;
	static int joyPadNum_;
	int frame;
	int id_;
private:
	virtual void Update(void) = 0;

};




