//------------------------------------------
// ����      �F����
//------------------------------------------
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include <DxLib.h>
#include "Application.h"

// WinMain�֐�
//---------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Application application;	//�C���X�^���X�̐���

	if (!application.Init())
	{
		return -1;
	}

	application.Run();

	if (!application.Release())
	{
		return -1;
	}

	return 0;

}
