#include "EventShot.h"
#include "SpriteAnimator.h"
#include "SceneManager.h"
#include "Camera.h"

EventShot::EventShot(SceneManager* manager, Transform* transform) :
	ShotBase(manager, transform)
{
}

void EventShot::Update(void)
{
	// 着弾時のエフェクト
	mExplosion->Update();

	if (!IsAlive())
	{
		return;
	}

	// 移動処理
	Move();

	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (pos2D.z <= 0 || pos2D.z >= 1)
	{
		VECTOR velocity = VScale(VScale(mDir, -1),100);
		mTransform.pos = VAdd(mTransform.pos, velocity);
		mTransform.Update();
		// 着弾エフェクト
		CreateExplosion();
		// カメラシェイクタイミング
		mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::SHAKE);
	}

	// モデル制御の基本情報更新
	mTransform.Update();


}

float EventShot::GetSpeed(void)
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void)
{
	return 10.0f;
}
