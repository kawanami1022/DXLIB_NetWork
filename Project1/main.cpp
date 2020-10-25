#include <DxLib.h>
#include "SceneMng.h"

// ƒvƒƒOƒ‰ƒ€‚Í WinMain ‚©‚çn‚Ü‚è‚Ü‚·
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	lpSceneMng.Run();
	lpSceneMng.Destroy();
	return 0;
}
