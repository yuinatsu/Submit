#include <DxLib.h>
#include "ResultScene.h"
#include "Transition/Fade.h"
#include "Transition/Loading.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "../Application.h"
#include "../Common/Camera.h"
#include "../Common/ResourceMng.h"
#include "../Common/Debug.h"
#include "../Common/SoundPross.h"
#include "../UI/UiManager.h"

ResultScene::ResultScene(ResultAttribute atr) :
	BaseScene{ ScreenID::Result,SceneID::Result }
{
	SetMakeSceneFunc(std::bind(&ResultScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
	SetMakeSceneFunc(std::bind(&ResultScene::MakeRetryFunc, this, std::placeholders::_1), SceneID::Game);
	
	SoundProcess::Init(SceneID::Result);
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::ResultSceneBGM, SoundProcess::GetVolume(), true);
	uiMng_ = std::make_unique<UiManager>(GetID(), atr);
	AddLoadedFunc([this](auto& contr) { 
		uiMng_->Begin(); 
		});
}

BaseScene::SceneUptr ResultScene::MakeSelectFunc(SceneUptr own)
{
	SoundProcess::Release();
	return std::make_unique<Fade>(std::move(own), std::make_unique<SelectScene>(), 2.0f);
}

BaseScene::SceneUptr ResultScene::MakeRetryFunc(SceneUptr own)
{
	SoundProcess::Release();
	return std::make_unique<Fade>(std::move(own), std::make_unique<Loading>(std::make_unique<GameScene>(), TransitionType::Fade, 2.0f), 2.0f);
}

void ResultScene::Update(float delta, Controller& controller)
{
	uiMng_->Update(delta, *this, *objMng_, controller);
}

void ResultScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	uiMng_->Draw();
}
