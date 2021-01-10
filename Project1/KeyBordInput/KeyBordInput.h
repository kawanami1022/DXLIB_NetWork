#pragma once
//“ú–{Œê“ü—Í
#include <string>
#include "../Lib/Vector2/Vector2.h"

class KeyBordInput
{
public:
	KeyBordInput();
	KeyBordInput(Position2);
	~KeyBordInput();

	std::string GetInputString();

	void Update();
	void Draw();
	void Reset();
private:
	void Init();

	int inputHandle_;
	Position2 drawStrPos_;
};

