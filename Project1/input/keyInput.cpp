#include <DxLib.h>
#include "keyInput.h"

ContType KeyInput::GetType(void)
{
	return ContType::Key;
}

bool KeyInput::Setup(int no)
{
	if (no == 0)
	{
		_InputTbl = {
			{InputID::Up,KEY_INPUT_UP},
			{InputID::Down,KEY_INPUT_DOWN},
			{InputID::Left,KEY_INPUT_LEFT},
			{InputID::Right,KEY_INPUT_RIGHT},
			{InputID::TURN_L,KEY_INPUT_K},
			{InputID::TURN_R,KEY_INPUT_L}
			//{InputID::SPACE,KEY_INPUT_SPACE},
		};
	}
	else
	{
		_InputTbl = {
			{InputID::Up,KEY_INPUT_W},
			{InputID::Down,KEY_INPUT_S},
			{InputID::Left,KEY_INPUT_A},
			{InputID::Right,KEY_INPUT_D},
			{InputID::TURN_L,KEY_INPUT_Q},
			{InputID::TURN_R,KEY_INPUT_E},
			//{InputID::SPACE,KEY_INPUT_SPACE},
		};
	}
	id_ = no;
	frame = 0;
	joyPadNum_ = GetJoypadNum();
	return true;
}

void KeyInput::Update(void)
{
	if (frame % 60 == 0)joyPadNum_ = ReSetupJoypad();
	GetHitKeyStateAll(_keyData.data());
	for (auto id:InputID())
	{
		_data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
		_data[id][static_cast<int>(Trg::Now)] = _keyData[_InputTbl[id]];
	}
	frame++;
}

bool KeyInput::push(InputID inputID)
{
	return (!_data[inputID][static_cast<int>(Trg::Old)] && _data[inputID][static_cast<int>(Trg::Now)]);
}

bool KeyInput::Hold(InputID)
{
	return false;
}

bool KeyInput::release(InputID)
{
	return false;
}

bool KeyInput::separate(InputID)
{
	return false;
}

void KeyInput::changeInputTbl(int inputButton,InputID inputID)
{
	_InputTbl[inputID] = inputButton;
}

void KeyInput::PadForceFeedback(int Power, int Time)
{
}

void KeyInput::DebugDrow(int id)
{
	
	if(id==0)
	DrawFormatString(0, 0, 0xffffff, "inputMode: KeyInput");
	if(id==1)
	DrawFormatString(400, 0, 0xffffff, "inputMode: KeyInput");
}

