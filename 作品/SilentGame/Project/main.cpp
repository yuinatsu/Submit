#include <DxLib.h>
#include "Scene/SceneMng.h"
#include "resource.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// アイコン変えたいときはここを変えてね
	SetWindowIconID(IDI_ICON1);

	lpSceneMng.Run();
	return 0;
}
