#include <cmath>
#include "DxLib.h"
#include "GameCommon.h"
#include "KeyCheck.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameScene.h"
#include "SpaceDome.h"
#include "Stage.h"
#include "Player.h"
#include "RockManager.h"
#include "SpeechBalloon.h"

namespace
{
	constexpr float RESTART_TIME = 2.0f;
}

GameScene::GameScene(SceneManager* manager) : SceneBase(manager)
{
}

void GameScene::Init(void)
{
	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();

	mPlayer = new Player(mSceneManager);
	mPlayer->Init();

	mStage = new Stage(mSceneManager, mPlayer);
	mStage->Init();

	rockManager = new RockManager(mSceneManager, mPlayer);
	rockManager->Init();

	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW_SPRING);
	mSceneManager->GetCamera()->SetPlayer(mPlayer);

	mSpaceDome->SetPlayer(mPlayer);

	playerDeadTime_ = RESTART_TIME;
}

void GameScene::Update(void)
{
	auto Collision = [](const int& handle,const VECTOR& center,const float& rad)
	{
		auto info = MV1CollCheck_Sphere(handle, -1, center, rad);
		bool isHit = info.HitNum != 0 ? true : false;
		MV1CollResultPolyDimTerminate(info);
		return isHit;
	};
	mSpaceDome->Update();
	mPlayer->Update();
	rockManager->Update();
	mStage->Update();
	if (mPlayer->isAlive())
	{
		if (Collision(mStage->GetModelHadle(), mPlayer->GetTransForm()->pos, Player::COLLISION_RADIUS))
		{
			mPlayer->Dead();
		}
	}
	else
	{
		if (mPlayer->isEnd())
		{
			playerDeadTime_ -= mSceneManager->GetDeltaTime();
			if (playerDeadTime_ <= 0.0f)
			{
				mSceneManager->ChangeScene(SceneManager::SCENE_ID::GAME, true);
			}
		}
	}


	// ƒV[ƒ“‘JˆÚ
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}

}

void GameScene::Draw(void)
{
	mSpaceDome->Draw();
	mStage->Draw();
	mPlayer->Draw();
	rockManager->Draw();
	// ‚±‚±‚©‚çUIŒn
	mPlayer->speechBalloon_->Draw();
}

void GameScene::Release(void)
{
	mSpaceDome->Release();
	mStage->Release();
	mPlayer->Release();
	rockManager->Release();
}
