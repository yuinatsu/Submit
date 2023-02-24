#include "OptionScene.h"
#include <DxLib.h>
#include "../UI/UiManager.h"

OptionScene::OptionScene() :
	BaseScene{ScreenID::Option, SceneID::Option}
{
	uiMng_ = std::make_unique<UiManager>(SceneID::Option);
}

void OptionScene::Update(float delta, Controller& controller)
{
	if (controller.Pressed(InputID::btn1))
	{
		ChangeBackScene();
	}
}

void OptionScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();

	DrawString(0, 0, L"ê›íËâÊñ Ç≈Ç∑",0xffffff);
}
