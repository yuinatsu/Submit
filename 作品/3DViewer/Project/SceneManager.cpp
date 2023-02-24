#include <chrono>
#include "DxLib.h"
#include "Fader.h"
#include "TitleScene.h"
#include "Camera.h"
#include "MiniCamera.h"
#include "AsoUtility.h"
#include "SceneManager.h"

void SceneManager::Init()
{

	sceneID_ = SCENE_ID::TITLE;
	waitSceneID_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	camera_ = new Camera();
	camera_->Init();

	miniCamera_ = new MiniCamera(camera_);
	miniCamera_->Init();

	scene_ = new TitleScene(this);
	scene_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	tickCount_ = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

}

void SceneManager::Init3D(void)
{

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// �J�����̃N���b�N�����̐ݒ�
	SetCameraNearFar(0.0f, 1500.0f);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8 });

	// 3D�̔w�i�F
	SetBackgroundColor(0, 139, 139);



}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto tickCount = std::chrono::system_clock::now();
	deltaTime_ = std::chrono::duration_cast<std::chrono::nanoseconds>(tickCount - tickCount_).count() / 1000000000.0f;
	tickCount_ = tickCount;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fader::FADE_STATE fState = fader_->GetState();
		switch (fState)
		{
		case Fader::FADE_STATE::FADE_IN:
			if (fader_->IsEnd())
			{
				fader_->SetFade(Fader::FADE_STATE::NONE);
				isSceneChanging_ = false;
			}
			break;
		case Fader::FADE_STATE::FADE_OUT:
			if (fader_->IsEnd())
			{
				DoChangeScene();
				fader_->SetFade(Fader::FADE_STATE::FADE_IN);
			}
			break;
		default:
			break;
		}
	}
	else 
	{
		scene_->Update();
	}

	// �J����
	camera_->Update();

	// �~�j�J����
	miniCamera_->Update();
	miniCamera_->DrawScreen();

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	camera_->SetBeforDraw();

	// �`��
	scene_->Draw();
	camera_->Draw();

	// �~�j�J����
	miniCamera_->Draw();

	fader_->Draw();

}

void SceneManager::Release(void)
{

	scene_->Release();
	delete scene_;

	camera_->Release();
	delete camera_;
}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{
	waitSceneID_ = nextId;

	if (isFading)
	{
		fader_->SetFade(Fader::FADE_STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else 
	{
		DoChangeScene();
	}

}

float SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void)
{
	return camera_;
}

void SceneManager::DoChangeScene(void)
{
	scene_->Release();

	sceneID_ = waitSceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene(this);
		break;
	default:
		break;
	}

	scene_->Init();

	waitSceneID_ = SCENE_ID::NONE;

}



