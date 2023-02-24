#include <DxLib.h>
#include "main.h"
#include "class/Scene/SceneMng.h"
#include "resource.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	DxLib::SetWindowIconID(IDI_ICON1);
	lpSceneMng.Run();
	return 1;
}