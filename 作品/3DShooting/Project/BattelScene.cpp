#include <DxLib.h>
#include "BattelScene.h"
#include "RockManager.h"
#include "Player.h" 
#include "BossShip.h"
#include "SpaceDome.h"
#include "PlayerShot.h"

#include "KeyCheck.h"
#include "SceneManager.h"
#include "Camera.h"

#include "Turret.h"
#include "TurretShot.h"

#include "AsoUtility.h"
namespace
{
	constexpr float RESTART_TIME = 2.0f;
}

BattelScene::BattelScene(SceneManager* manager) :
	SceneBase(manager)
{
}

void BattelScene::Init(void)
{
	player_ = new Player(mSceneManager);
	player_->Init();
	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();
	mSpaceDome->SetPlayer(player_);
	rockManager_ = new RockManager(mSceneManager, player_);
	rockManager_->Init();
	rockManager_->Update();
	boss_ = new BossShip(mSceneManager, player_);
	boss_->Init();
	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW_SPRING);
	mSceneManager->GetCamera()->SetPlayer(player_);

	playerDeadTime_ = RESTART_TIME;
}

void BattelScene::Update(void)
{
	auto Collision = [](const int& handle, const VECTOR& center, const float& rad)
	{
		auto info = MV1CollCheck_Sphere(handle, -1, center, rad);
		bool isHit = info.HitNum != 0 ? true : false;
		MV1CollResultPolyDimTerminate(info);
		return isHit;
	};
	// ƒV[ƒ“‘JˆÚ
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}
	player_->Update();
	boss_->Update();
	mSpaceDome->Update();
	rockManager_->Update();

	if (player_->isAlive())
	{
		auto info = MV1CollCheck_Sphere(boss_->GetModelID(), -1, player_->GetTransForm()->pos, Player::COLLISION_RADIUS);

		if (info.HitNum)
		{
			player_  ->Dead();
		}
		MV1CollResultPolyDimTerminate(info);

		auto turrets = boss_->GetTurret();
		for (const auto& t:turrets)
		{
			if (!t->IsAlive())
			{
				continue;
			}
			for (const auto&s:t->GetShots())
			{
				if (!s->IsAlive())
				{
					continue;
				}
				if (AsoUtility::IsHitSpheres(player_->GetTransForm()->pos, Player::COLLISION_RADIUS, s->GetPos(), s->GetCollisionRadius()))
				{
					player_->Dead();
					s->CreateExplosion();
				}
			}
			for (const auto& ps : player_->GetShots())
			{
				if (!ps->IsAlive())
				{
					continue;
				}
				if (AsoUtility::IsHitSpheres(ps->GetPos(), ps->GetCollisionRadius(), t->GetPos(), Turret::COLLISION_RADIUS))
				{
					ps->CreateExplosion();
					t->Damage();
				}
			}
		}
	}
	else
	{
		if (player_->isEnd())
		{
			playerDeadTime_ -= mSceneManager->GetDeltaTime();
			if (playerDeadTime_ <= 0.0f)
			{
				mSceneManager->ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
			}
		}
	}
	auto shots = player_->GetShots();

	for (auto shot : shots)
	{
		if (Collision(boss_->GetModelID(), shot->GetPos(), PlayerShot::COLLISION_RADIUS))
		{
			shot->CreateExplosion();
		}
	}
}

void BattelScene::Draw(void)
{
	mSpaceDome->Draw();
	boss_->Draw();
	player_->Draw();
	rockManager_->Draw();
}

void BattelScene::Release(void)
{
	player_->Release();
	delete player_;
	boss_->Release();
	delete boss_;
	mSpaceDome->Release();
	delete mSpaceDome;
	rockManager_->Release();
	delete rockManager_;
}
