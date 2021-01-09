#pragma once
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
private:
	void Init();
	int inputHandle_;
	Position2 drawStrPos_;
};

