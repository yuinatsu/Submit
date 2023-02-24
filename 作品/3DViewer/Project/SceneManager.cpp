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

	// デルタタイム
	tickCount_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// カメラのクリック距離の設定
	SetCameraNearFar(0.0f, 1500.0f);

	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8 });

	// 3Dの背景色
	SetBackgroundColor(0, 139, 139);



}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
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

	// カメラ
	camera_->Update();

	// ミニカメラ
	miniCamera_->Update();
	miniCamera_->DrawScreen();

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforDraw();

	// 描画
	scene_->Draw();
	camera_->Draw();

	// ミニカメラ
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



