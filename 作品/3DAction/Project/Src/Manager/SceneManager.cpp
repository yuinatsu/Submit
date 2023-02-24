#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Common/Fader.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "Camera.h"
#include "SceneManager.h"

void SceneManager::Init()
{

	// 3D�p�̐ݒ�
	Init3D();

	// Effekseer�̐ݒ�
	InitEffekseer();

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mResourceManager = new ResourceManager();
	mResourceManager->Init();

	mFader = new Fader();
	mFader->Init();

	// �d�͊Ǘ�(Camera�����)
	mGravityManager = new GravityManager(this);
	mGravityManager->Init();

	mCamera = new Camera(this);
	mCamera->Init();

	mScene = new TitleScene(this);
	mScene->Init();

	mIsSceneChanging = false;

	// �f���^�^�C��
	mTickCount = std::chrono::system_clock::now();


}

void SceneManager::Init3D(void)
{

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	// �t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::InitEffekseer(void)
{
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// Effekseer�̏�����
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

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

	// �d�͕����̌v�Z
	mGravityManager->Update();

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

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	mCamera->SetBeforeDraw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V
	UpdateEffekseer3D();

	// �`��
	mScene->Draw();
	mCamera->Draw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`��
	DrawEffekseer3D();

	// �Ō�
	mFader->Draw();
	
}

void SceneManager::Release(void)
{

	mScene->Release();
	delete mScene;

	mCamera->Release();
	delete mCamera;

	delete mGravityManager;

	mResourceManager->Release();
	delete mResourceManager;

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

float SceneManager::GetDeltaTime(void)
{
	//return 1.0f / 60.0f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void)
{
	return mCamera;
}

ResourceManager* SceneManager::GetResourceManager(void)
{
	return mResourceManager;
}

GravityManager* SceneManager::GetGravityManager(void)
{
	return mGravityManager;
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
	delete mScene;

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene(this);
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene(this);
		break;
	}

	mScene->Init();

	ResetDeltaTime();

	mWaitSceneID = SCENE_ID::NONE;

}
