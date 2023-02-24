#include <DxLib.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "Transition/CrossOver.h"
#include "Transition/FadeInOut.h"
#include "Transition/Slide.h"
#include "../Mng/ImageMng.h"
#include "../Mng/SceneMng.h"

#define TITLE_SIZEX (776)
#define TITLE_SIZEY (350)
#define SPACE_SIZEX (400)
#define SPACE_SIZEY (60)

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init(void)
{
	lpImageMng.GetID("bg", "Resource/Image/bg.jpg");
	lpImageMng.GetID("title", "Resource/Image/title.png");
	lpImageMng.GetID("space", "Resource/Image/space.png");
	screenSize_ = lpSceneMng.GetScreenSize();
	return true;
}

const SceneID TitleScene::GetSceneID(void) const
{
	return SceneID::Title;
}

std::unique_ptr<BaseScene> TitleScene::Update(const double& delta, std::unique_ptr<BaseScene> ownScene)
{
	if (CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(PAD_INPUT_1))
	{
		return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<GameScene>());
	}
	return std::move(ownScene);
}

void TitleScene::DrawOwnScreen(const double& delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("bg")[0], true);
	DrawGraph((screenSize_.x - TITLE_SIZEX) / 2, 0, lpImageMng.GetID("title")[0], true);
}
