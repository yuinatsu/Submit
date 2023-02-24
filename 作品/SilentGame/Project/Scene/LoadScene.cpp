#include <DxLib.h>
#include <random>
#include <string>
#include "LoadScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "../common/ImageMng.h"
#include "Transition/CircleScene.h"
#include "Transition/FadeTrans.h"
#include "SceneMng.h"
#include "Transition/BlendTrans.h"

LoadScene::LoadScene(double limitTime, uniqueScene before,uniqueScene after) :
	before_{std::move(before)}, after_{std::move(after)}
{
	limitTime_ = limitTime;

	Init();
	DrawOwnScreen(0.0);
}

LoadScene::~LoadScene()
{
}

void LoadScene::LoadPic(void)
{
	lpImageMng.GetID("Resource/Image/LoadScene/laodPic_kakashi.png", "laodPic_kakashi");
	lpImageMng.GetID("Resource/Image/LoadScene/laodPic_sake.png", "loadPic_sake");
	lpImageMng.GetID("Resource/Image/LoadScene/laodPic_bomb.png", "loadPic_bomb");
	lpImageMng.GetID("Resource/Image/LoadScene/laodPic_patrol.png", "loadPic_patrol");
	lpImageMng.GetID("Resource/Image/LoadScene/laodPic_tokei.png", "loadPic_tokei");
}

bool LoadScene::Init(void)
{
	// 画像のロード
	LoadPic();
	lpImageMng.GetID("Resource/Image/LoadScene/LoadingString.png", "LoadingString");
	lpImageMng.GetID("Resource/Image/LoadScene/loadEndString.png", "LoadEndString");
	return true;
}

uniqueScene LoadScene::Update(float delta, uniqueScene ownScene)
{
	DrawOwnScreen(delta);
	count += delta;

	// 次のシーンがゲームシーンだったらステージ開始用のトランディション
	if (after_->GetSceneID() == Scene::Game)
	{
		if (count > limitTime_ && GetASyncLoadNum() <= 0)
		{
			stringFlag = false;

			if (CheckHitKeyAll())
			{
				after_->Loaded();
				return std::make_unique<BlendTrans>(
					3.0,
					std::move(ownScene),
					std::move(after_)
					);
			}
		}
	}
	//else if (after_->GetSceneID() == Scene::Clear)
	//{
	//	if (count > limitTime_ && GetASyncLoadNum() <= 0)
	//	{
	//		after_->Loaded();
	//		return std::make_unique<ClearScene>();
	//	}
	//}
	// それ以外のシーンはフェード
	else
	{
		if (count > limitTime_ && GetASyncLoadNum() <= 0)
		{
			after_->Loaded();
			return std::make_unique<FadeTrans>(
				2.0,
				std::move(ownScene),
				std::move(after_)
				);
		}
	}

	return ownScene;
}

void LoadScene::DrawOwnScreen(float delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	// フェードイン
	auto alpha = static_cast<int>(255 * count);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	before_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawLoadPic(delta);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}

void LoadScene::DrawLoadPic(double delta)
{
	if (stringFlag)
	{
		if (((stringCount_ / 10) % 2) == 0)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x - 250, lpSceneMng.GetScreenSize().y - 100, 0.5, 0.0, lpImageMng.GetID("LoadingString")[0], true);
		}
	}
	stringCount_++;
	if (stringFlag == false)
	{
		DrawRotaGraph(lpSceneMng.GetScreenSize().x - 520, lpSceneMng.GetScreenSize().y - 350, 1.0, 0.0, lpImageMng.GetID("LoadEndString")[0], true);
	}

	// 画像の切り替わり
	if (after_->GetSceneID() == Scene::Game && stringFlag == true)
	{
		time += delta;
		if (time <= 1.5)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x - 400, lpSceneMng.GetScreenSize().y - 400, 1.5, 0.0,lpImageMng.GetID("laodPic_kakashi")[0], true);
		}
		else if (time >= 1.5 && time <= 3.0)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x - 400, lpSceneMng.GetScreenSize().y - 400, 1.5, 0.0, lpImageMng.GetID("loadPic_sake")[0], true);
		}
		else if (time >= 3.0 && time <= 4.5)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x - 400, lpSceneMng.GetScreenSize().y - 400, 1.5, 0.0, lpImageMng.GetID("loadPic_bomb")[0], true);
		}
		else if (time >= 4.5 && time <= 6.0)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x - 400, lpSceneMng.GetScreenSize().y - 400, 1.5, 0.0, lpImageMng.GetID("loadPic_patrol")[0], true);
		}
		else if (time >= 6.0)
		{
			time = 0.0;
		}
	}
}

