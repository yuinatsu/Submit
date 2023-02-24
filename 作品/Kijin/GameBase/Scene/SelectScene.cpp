#include "SelectScene.h"
#include "Transition/Fade.h"
#include "Transition/Loading.h"
#include "GameScene.h"
#include "OptionScene.h"
#include "../UI/UiManager.h"

SelectScene::SelectScene() : BaseScene{ ScreenID::Select,SceneID::Select }
{
	SetMakeSceneFunc(std::bind(&SelectScene::MakeGameFunc, this, std::placeholders::_1), SceneID::Game);
	SetMakeSceneFunc(std::bind(&SelectScene::MakeOptionFunc, this, std::placeholders::_1), SceneID::Option);
	uiMng_ = std::make_unique<UiManager>(GetID());
	AddLoadedFunc([this](auto& contr) {
		uiMng_->Begin();
		});
}

BaseScene::SceneUptr SelectScene::MakeGameFunc(SceneUptr own)
{
	return std::make_unique<Fade>(std::move(own), std::make_unique<Loading>(std::make_unique<GameScene>(), TransitionType::Fade, 2.0f), 2.0f);
}

BaseScene::SceneUptr SelectScene::MakeOptionFunc(SceneUptr own)
{
	return std::make_unique<Fade>(std::move(own), std::make_unique<OptionScene>(), 2.0f);
}

void SelectScene::Update(float delta, Controller& controller)
{
	uiMng_->Update(delta, *this, *objMng_, controller);
	if (controller.Pressed(InputID::Jump))
	{
		ChangeSceneID(SceneID::Game);
		return;
	}
	if (controller.Pressed(InputID::btn1))
	{
		ChangeSceneID(SceneID::Option);
		return;
	}
}

void SelectScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	uiMng_->Draw();
}
