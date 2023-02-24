#include <DxLib.h>
#include "TransitionScene.h"

TransitionScene::TransitionScene(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene)
{
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	afterScene_->DrawOwnScreen(0.0);
	OutputDebugString("TransitionSceneÇÃê∂ê¨\n");
	count_ = 0.0;
	Init();
}

TransitionScene::~TransitionScene()
{
}

const SceneID TransitionScene::GetSceneID(void) const
{
	return SceneID::Transition;
}

bool TransitionScene::Init(void)
{
	return true;
}

std::unique_ptr<BaseScene> TransitionScene::Update(const double& delta, std::unique_ptr<BaseScene> ownScene)
{
	if (UpdateTransition(delta))
	{
		return std::move(afterScene_);
	}
	return ownScene;
}
