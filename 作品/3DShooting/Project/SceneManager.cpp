#include <chrono>
#include "DxLib.h"
#include "Fader.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EventScene.h"
#include "BattelScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ParticleGenerator.h"

void SceneManager::Init()
{

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera(this);
	mCamera->Init();

	mResourceManager = new ResourceManager();
	mResourceManager->Init();

	particleGenerator_ = new ParticleGenerator(this, { 0,0,0 }, 20);
	particleGenerator_->Init();

	mScene = new TitleScene(this);
	mScene->Init();

	mIsSceneChanging = false;

	// �f���^�^�C��
	mTickCount = std::chrono::system_clock::now();

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

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	//SetCameraNearFar(0.0f, 50.0f);

	// ���C�g�̐ݒ�
	//ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(false);

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(0, 100, 100);

	// �J�����ݒ�
	// ������Ԃł͎��_�̈ʒu���A 
	// x = 320.0f, y = 240.0f, z = (��ʂ̃T�C�Y�ɂ���ĕω�)
	// �����_�̈ʒu�́A
	// x = 320.0f, y = 240.0f, z = 1.0f�A
	// �J�����̏������
	// x = 0.0f, y = 1.0f, z = 0.0f
	// �܂��ʂ̂w�x���ʏ�̒��S�ɋ��Ăy���̃v���X����������悤�ȃJ�����ɂȂ��Ă��܂��B
	//SetCameraPositionAndAngle(
	//	{ 0.0f, 200, -500.0f },
	//	30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f
	//);

	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(3000.0f, 15000.0f);

}

void SceneManager::Update(void)
{

	if (mScene == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto tickCount = std::chrono::system_clock::now();
	mDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tickCount - mTickCount).count() / 1000000000.0f;
	mTickCount = tickCount;

	mFader->Update();
	if (mIsSceneChanging)
	{
		Fader::FADE_STATE fState = mFader->GetState();
		switch (fState)
		{
		case Fader::FADE_STATE::FADE_IN:
			if (mFader->IsEnd())
			{
				mFader->SetFade(Fader::FADE_STATE::NONE);
				mIsSceneChanging = false;
			}
			break;
		case Fader::FADE_STATE::FADE_OUT:
			if (mFader->IsEnd())
			{
				DoChangeScene();
				mFader->SetFade(Fader::FADE_STATE::FADE_IN);
			}
			break;
		default:
			break;
		}
	}
	else 
	{
		mScene->Update();
	}

	// �J�����X�V�X�e�b�v
	mCamera->Update();

	//particleGenerator_->Update();

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	mCamera->SetBeforeDraw();

	// �`��
	mScene->Draw();
	mCamera->Draw();
	//particleGenerator_->Draw();

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		break;
	case SCENE_ID::GAME:
		// �f�o�b�O�`��
		mCamera->DrawDebug();
		// UI�`��
		mCamera->DrawUI();
		break;
	}

	mFader->Draw();

}

void SceneManager::Release(void)
{

	delete mFader;

	mScene->Release();
	delete mScene;

	mCamera->Release();
	delete mCamera;

	mResourceManager->Release();
	delete mResourceManager;

	particleGenerator_->Release();
	delete particleGenerator_;
}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	mWaitSceneID = nextId;

	if (isFading)
	{
		mFader->SetFade(Fader::FADE_STATE::FADE_OUT);
		mIsSceneChanging = true;
	}
	else 
	{
		DoChangeScene();
	}

}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return mSceneID;
}

float SceneManager::GetDeltaTime(void)
{
	//return 0.016f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void)
{
	return mCamera;
}

ResourceManager* SceneManager::GetResource(void)
{
	return mResourceManager;
}

void SceneManager::ResetDeltaTime(void)
{
	mDeltaTime = 0.016f;
	mTickCount = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	mResourceManager->Release();
	mScene->Release();

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene(this);
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene(this);
		break;
	case SCENE_ID::EVENT:
		mScene = new EventScene(this);
		break;
	case SCENE_ID::BATTLE:
		mScene = new BattelScene(this);
		break;
	default:
		break;
	}

	mScene->Init();
	
	ResetDeltaTime();

	mWaitSceneID = SCENE_ID::NONE;
}



