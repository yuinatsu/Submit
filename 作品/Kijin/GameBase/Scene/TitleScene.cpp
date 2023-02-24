#include <DxLib.h>
#include "GameScene.h"
#include "Transition/Loading.h"
#include "Transition/Fade.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Factory/PlayerFactory.h"
#include "../Object/ObjectManager.h"
#include "../Common/Camera.h"
#include "../Common/ResourceMng.h"
#include "../Common/SoundPross.h"
#include "SelectScene.h"
#include "../UI/UiManager.h"

TitleScene::TitleScene() :
	BaseScene{ScreenID::Title,SceneID::Title}
{
	SetMakeSceneFunc(std::bind(&TitleScene::MakeGameFunc, this, std::placeholders::_1), SceneID::Select);
	//objMng_ = std::make_unique<ObjectManager>(10);

	SoundProcess::Init(SceneID::Title);
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::TitleSceneBGM, SoundProcess::GetVolume(), true);

	uiMng_ = std::make_unique<UiManager>(GetID());
	AddLoadedFunc([this](auto& contr) {
		uiMng_->Begin();
		});
}

BaseScene::SceneUptr TitleScene::MakeGameFunc(SceneUptr own)
{
	return std::make_unique<Fade>(std::move(own), std::make_unique<Loading>(std::make_unique<SelectScene>(), TransitionType::Fade, 2.0f, false), 2.0f);
}

void TitleScene::Update(float delta, Controller& controller)
{
	uiMng_->Update(delta, *this, *objMng_, controller);
	if (controller.Pressed(InputID::Jump))
	{
		ChangeSceneID(SceneID::Select);
		SoundProcess::Release();
	}
	//objMng_->Update(delta, *this);
}

void TitleScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	camera_->SetUpScreen();
	uiMng_->Draw();

	// 影の描画

	// 描画

	// ポストエフェクト

	//objMng_->Draw();
	// ここに直書きしているが後から変えること
	//DrawFormatString(0, 0, 0xffffff, TEXT("%dIDのシーンです"), static_cast<unsigned int>(scID_));
}

