#include <DxLib.h>
#include "SceneMng.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	lpSceneMng.Run();
	lpSceneMng.Destroy();
	return 0;
}
