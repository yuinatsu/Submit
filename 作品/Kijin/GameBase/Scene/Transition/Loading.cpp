#include <DxLib.h>
#include "../../Application.h"
#include "Fade.h"
#include "Loading.h"
#include "../../Common/Debug.h"
#include "../../UI/UiManager.h"

#include <EffekseerForDXLib.h>

Loading::Loading(SceneUptr loadScene, TransitionType type, float transTime, bool isIndicator) :
    BaseScene{ScreenID::Loading, SceneID::Loading}, type_{type}, loadScene_{std::move(loadScene)}, transTime_{transTime},
	isIndicator_{isIndicator}
{
	SetMakeSceneFunc(std::bind(&Loading::MakeNextFunc, this, std::placeholders::_1), SceneID::Transition);
	uiMng_ = std::make_unique<UiManager>(SceneID::Loading);
	uiMng_->Begin();
}

void Loading::Update(float delta, Controller& controller)
{
	if (loadScene_->IsLoaded())
	{
		loadScene_->Loaded(controller);
		ChangeSceneID(SceneID::Transition);
	}
	uiMng_->Update(delta, *this, *objMng_, controller);
}

void Loading::LoadingIndicator(float delta)
{
}

void Loading::Relese(void)
{
}

void Loading::DrawScene(void)
{
	if (isIndicator_)
	{
		SetDrawScreen(*screenHandle_);
		ClsDrawScreen();
		uiMng_->Draw();
	}
}

BaseScene::SceneUptr Loading::MakeNextFunc(SceneUptr own)
{
	switch (type_)
	{
	case TransitionType::Fade:
		return std::make_unique<Fade>(std::move(own), std::move(loadScene_), transTime_);
	default:
		return std::move(own);
	}
}
