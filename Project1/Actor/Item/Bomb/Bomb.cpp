#include <DxLib.h>
#include "../../../Manager/ImageManager.h"
#include "Bomb.h"

Bomb::Bomb():time_(0.f)
{
	IpImage.LoadDivTexture("BombImage", "Image/bomb.png", 1, 1, 1);
}

Bomb::~Bomb()
{
}

void Bomb::Update()
{
}

void Bomb::Draw()
{
	DrawGraph(0, 0, IpImage.GetTxHdl("BombImage").second[0], true);
}
