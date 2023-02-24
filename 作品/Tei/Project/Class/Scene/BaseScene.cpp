#include <DxLib.h>
#include "BaseScene.h"
#include "../Mng/SceneMng.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
	
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw(const double& delta)
{
	DrawGraph(0, 0, screenID_, true);
}

