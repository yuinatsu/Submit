#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Common/GameCommon.h"
#include "Common/KeyCheck.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Application.h"

bool Application::Init(void)
{

	// �V�X�e������
	SetWindowText("Project");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib �̏��������s");
		return false;
	}

	KeyInit();
	mSceneManager.Init();

	// �f�o�b�O
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	return true;

}

void Application::Run(void)
{
	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
		_dbgStartDraw();

		CheckKeyList();

		mSceneManager.Update();

		_dbgAddDraw();

		ScreenFlip();

	}
}

bool Application::Release(void)
{
	mSceneManager.Release();

	// Effekseer���I������
	Effkseer_End();

	DxLib_End();

	return true;
}
