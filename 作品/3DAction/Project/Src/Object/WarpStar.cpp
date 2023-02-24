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

	// ���f���̊�{���
	mTransform.SetModel(
		mSceneManager->GetResourceManager()->LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);
	mTransform.Update();

	// Z������]�̉�]��Ԃ�ێ����Ă���
	VECTOR angles = mTransform.quaRot.ToEuler();
	mWarpQua = Quaternion::Euler(angles.x, angles.y, 0.0f);

	// ���[�v�����p�̈ړ����W
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
	// ��]
	RotateZ(SPEED_ROT_IDLE);

	// ����
	VECTOR sPos = mTransform.pos;
	// �_
	VECTOR pPos = mPlayer->GetCapsule()->GetCenter();
	// ���̂Ɠ_�̏Փ˔���
	VECTOR diff = VSub(pPos, sPos);
	float dis = AsoUtility::SqrMagnitude(diff);
	if (dis < RADIUS * RADIUS)
	{
		// �͈͂ɓ�����
		ChangeState(STATE::RESERVE);
		return;
	}
}

void WarpStar::UpdateReserve(void)
{
	// ��]
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
		// �v���C���[�̏�ԕύX
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
