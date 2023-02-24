#include <DxLib.h>
#include "TransitionScene.h"
#include "../../Object/ObjManager.h"

TransitionScene::TransitionScene(uniqueScene beforScene, uniqueScene afterScene)
{
	// 二つのシーンの保存
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	// カウントリセット
	count = 0;
	SceneFlag_ = false;
	// 描画
	Draw();
}

TransitionScene::~TransitionScene()
{
}

bool TransitionScene::Init(void)
{
	return false;
}

uniqueScene TransitionScene::Update(float delta, uniqueScene ownScene)
{
	// トランジション完了で次のシーンへ以降
	if (UpdataTransition(delta))
	{
		return std::move(afterScene_);
	}
	// 未完了ならトランジション描画
	DrawOwnScreen(delta);
	return ownScene;
}

void TransitionScene::Draw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw(0);
}
