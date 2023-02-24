#include <DxLib.h>
#include "TransitionScene.h"
#include "../SceneMng.h"
#include "../../../_debug/_DebugConOut.h"


TransitionScene::TransitionScene(uniqueBaseScene beforScene, uniqueBaseScene afterScene)
{
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	Init();
}


TransitionScene::~TransitionScene()
{

}


bool TransitionScene::Init(void)
{
	count_ = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw(0);
	return true;
}


uniqueBaseScene TransitionScene::Update(double delta, uniqueBaseScene own)
{
	if (UpdateTransition(delta))
	{
		return std::move(afterScene_);
	}
	DrawOwnScreen(delta);
	return own;
}
