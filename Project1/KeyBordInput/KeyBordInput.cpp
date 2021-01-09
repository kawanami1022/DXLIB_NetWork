#include <DxLib.h>
#include "KeyBordInput.h"

KeyBordInput::KeyBordInput() :drawStrPos_(Position2(0,32))
{
	Init();
}

KeyBordInput::KeyBordInput(Position2 pos):drawStrPos_(pos)
{
	Init();
}

KeyBordInput::~KeyBordInput()
{
	DeleteKeyInput(inputHandle_);
}

std::string KeyBordInput::GetInputString()
{
	std::string inputString;
	GetKeyInputString(inputString.data(), inputHandle_);
	return inputString;
}

void KeyBordInput::Update()
{
#ifdef DEBUG
	auto inputStrSize = inputString_.length();
	if (inputStrSize < 20 || CheckKeyInput(KEY_INPUT_BACK))
#endif // DEBUG

	auto inputStrSize = GetKeyInputCursorPosition(inputHandle_);
	//GetKeyInputString(inputString_.data(), inputHandle_);
}

void KeyBordInput::Draw()
{
	
	DrawString(drawStrPos_.x, drawStrPos_.y, "IpƒAƒhƒŒƒX“ü—Í‚µ‚Ä‚­‚¾‚³‚¢:", GetColor(255, 255, 255));
	DrawKeyInputString(drawStrPos_.x+15*16, drawStrPos_.y, inputHandle_);
	//DrawString(0, 32, inputString_.data(), GetColor(255, 255, 255));
}

void KeyBordInput::Init()
{
	inputHandle_ = MakeKeyInput(300, true, true, FALSE);
	SetActiveKeyInput(inputHandle_);
	SetIMEInputStringMaxLength(20);
}
