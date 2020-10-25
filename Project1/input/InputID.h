#pragma once


// ƒ{ƒ^ƒ“‚ÌŽí—Þ
enum class InputID
{
	Up,
	Down,
	Left,
	Right,
	TURN_L,
	TURN_R,
	//SPACE,
	Max
};

static InputID begin(InputID) { return InputID::Up; };
static InputID end(InputID) { return InputID::Max; };
static InputID operator++(InputID& state) { return (state = InputID(std::underlying_type<InputID>::type(state) + 1)); };
static InputID operator*(InputID state) { return state; }