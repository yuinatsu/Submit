#include <DxLib.h>
#include <memory>
#include <chrono>
#include "SceneMng.h"
#include "ViewerScene.h"
#include "../Common/Debug.h"
#include "../Input/InputConfig.h"

// �X�N���[���T�C�Y
static constexpr Vector2I screenSize{ 1280, 720 };

void SceneMng::Run(void)
{
	// �V�X�e���n������
	if (!SysInit())
	{
		return;
	}
	// ����������
	Init();
	// ���C�����[�v
	while (ProcessMessage() == 0)
	{
		// �X�V����
		Update();
		// �`�揈��
		Draw();
	}
	// �I������
	End();
}

bool SceneMng::SysInit(void)
{
	// �f�o�b�O�J�n	Debug.h
	DebugStart(time_);
	// �E�B���h�E�e�L�X�g
	SetWindowText("3DViewer");
	// ��ʃ��[�h�ݒ�
	SetGraphMode(screenSize.x, screenSize.y, 32);
	// �E�B���h�E���[�h
	ChangeWindowMode(true);
	// Direct3D�̃o�[�W�����ݒ�
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	// Z�o�b�t�@���g�p���邩
	SetUseZBuffer3D(true);
	// Z�o�b�t�@�ɏ������݂��s����
	SetWriteZBuffer3D(true);

	// DXLIB�̏�����
	if (DxLib_Init() == -1)
	{
		return false;
	}
	// �L�[�R���t�B�O����
	InputConfig::Create();
	// �f�o�b�O�Z�b�g�A�b�v	Debug.h
	DebugSetUp();

	return true;
}

void SceneMng::Init(void)
{
	// �V�[�������ݒ�
	scene_ = std::make_unique<ViewerScene>();

	// �f���^�^�C��������
	time_.DeltaTimeStart();
	time_.DeltaTimeEnd();
	time_.GameTimeEnd();
}

void SceneMng::Update(void)
{
	time_.DeltaTimeStart();

	// �V�[���X�V
	scene_ = scene_->Update(std::move(scene_), time_.GetDeltaTime<float>());

	time_.GameTimeEnd();
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	// �V�[���`��
	scene_->Draw();
	// �f�o�b�O���`��
	DebugInfoDraw();

	ScreenFlip();
	time_.DeltaTimeEnd();
}

void SceneMng::End(void)
{
	// �S�ẴO���t�B�b�N�n���h�����폜
	InitGraph();
	// �L�[�R���t�B�O�̍폜
	InputConfig::Destroy();
	// DXLIB�g�p�I��
	DxLib_End();
}
