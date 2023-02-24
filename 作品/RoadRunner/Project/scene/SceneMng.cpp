#include "SceneMng.h"
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"
#include "EditScene.h"
#include "GameScene.h"


void SceneMng::Init(void)
{
	SysInit();										// �V�X�e���n		// �d�v�ȓ��e���������܂�Ă���̂ŃR���X�g���N�^���ɏ����Ă��悢
	scene_ = std::make_unique<EditScene>();			// �����V�[���ݒ�
}


void SceneMng::Run(void)
{
	Init();											// ������

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		scene_ = scene_->Update(std::move(scene_));					// �V�[���X�V		// std::move ... �i�j���̂��̂��ړ�
		Draw();														// �V�[���`��
	}
}


void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);			// �`�悷���ʂ𗠂̉�ʂɐݒ�
	ClearDrawScreen();						// �`�悵����ʂ��N���A

	scene_->Draw();							// �`��

	_dbgAddDraw();							// �f�o�b�O�`��
	ScreenFlip();
}


// �V�X�e���n������
//-------------------
bool SceneMng::SysInit(void)
{
	// �V�X�e������
	SetWindowText("���[�h�����i�[");						// �Q�[���E�B���h�E�̃^�C�g��
	SetGraphMode(scnArea_.x, scnArea_.y, 16);				// �Q�[���E�B���h�E�̃T�C�Y�ƐF���[�h��ݒ�
	ChangeWindowMode(true);									// �Q�[���E�B���h�E�̕\�����@ (true : window   false : �t���X�N���[��)

	if (DxLib_Init() == -1)									// DX���C�u�����̏���������
	{
		TRACE("DxLib �̏��������s");
		return false;										// DX���C�u�����̏��������s�̈׃V�X�e���I��
	}

	SetDrawScreen(DX_SCREEN_BACK);							// �`����ʂ�ݒ�
	_dbgSetup(scnArea_.x, scnArea_.y, 255);					// �f�o�b�O��ݒ�

	return true;
}


#define VIEW_AREA_X 21										// �X�e�[�W���̕`��G���AX���W
#define VIEW_AREA_Y 17										// �X�e�[�W���̕`��G���AY���W


SceneMng::SceneMng() : scnArea_{ 800,600 }, worldArea_{ VIEW_AREA_X * 2,VIEW_AREA_Y }, viewArea_{ VIEW_AREA_X,VIEW_AREA_Y }, chipSize_{ 32,32 }, drawOffset_{ (scnArea_ - (viewArea_ * chipSize_)) / 2 }					// �X�N���[���G���A�T�C�Y��`
{

}


SceneMng::~SceneMng()
{

}
