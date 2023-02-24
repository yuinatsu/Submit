#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "Common/Capsule.h"
#include "Player.h"
#include "WarpStar.h"

WarpStar::WarpStar(SceneManager* manager, Player* player, Transform transform)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mPlayer = player;
	mTransform = transform;

	mState = STATE::NONE;
}

void WarpStar::Init(void)
{

	// モデルの基本情報
	mTransform.SetModel(
		mSceneManager->GetResourceManager()->LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);
	mTransform.Update();

	// Zが無回転の回転状態を保持しておく
	VECTOR angles = mTransform.quaRot.ToEuler();
	mWarpQua = Quaternion::Euler(angles.x, angles.y, 0.0f);

	// ワープ準備用の移動座標
	mWarpReservePos = VAdd(mTransform.pos, mWarpQua.PosAxis(WARP_RELATIVE_POS));

	ChangeState(STATE::IDLE);

}

void WarpStar::Update(void)
{

	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		UpdateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		UpdateReserve();
		break;
	case WarpStar::STATE::MOVE:
		UpdateMove();
		break;
	}

}

void WarpStar::UpdateIdle(void)
{
	// 回転
	RotateZ(SPEED_ROT_IDLE);

	// 球体
	VECTOR sPos = mTransform.pos;
	// 点
	VECTOR pPos = mPlayer->GetCapsule()->GetCenter();
	// 球体と点の衝突判定
	VECTOR diff = VSub(pPos, sPos);
	float dis = AsoUtility::SqrMagnitude(diff);
	if (dis < RADIUS * RADIUS)
	{
		// 範囲に入った
		ChangeState(STATE::RESERVE);
		return;
	}
}

void WarpStar::UpdateReserve(void)
{
	// 回転
	RotateZ(SPEED_ROT_RESERVE);

	if (mPlayer->IsWarpMove())
	{
		ChangeState(STATE::MOVE);
	}
}

void WarpStar::UpdateMove(void)
{
}

void WarpStar::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void WarpStar::Release(void)
{
}

Transform* WarpStar::GetTransform(void)
{
	return &mTransform;
}

void WarpStar::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		break;
	case WarpStar::STATE::RESERVE:
		// プレイヤーの状態変更
		mPlayer->StartWarp(TIME_WARP_RESERVE, mWarpQua, mWarpReservePos);
		break;
	case WarpStar::STATE::MOVE:
		break;
	}

}

void WarpStar::RotateZ(float speed)
{
	mTransform.quaRot = mTransform.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(speed), AsoUtility::AXIS_Z));
	mTransform.Update();
}
