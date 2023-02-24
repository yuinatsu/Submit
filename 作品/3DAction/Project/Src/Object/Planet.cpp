#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GravityManager.h"
#include "Common/Transform.h"
#include "WarpStar.h"
#include "Planet.h"

Planet::Planet(SceneManager* manager, Stage::NAME name, TYPE type, Transform transform)
{
    mSceneManager = manager;
	mGravityManager = manager->GetGravityManager();
	mName = name;
	mType = type;
	mTransform = transform;

	mGravityPow = 0.0f;
	mGravityRadius = 0.0f;
	mDeadLength = 0.0f;
}

void Planet::Init(void)
{
	mGravityPow = DEFAULT_GRAVITY_POW;
	mGravityRadius = DEFAULT_GRAVITY_RADIUS;
	mDeadLength = DEFAULT_DEAD_LENGTH;
}

void Planet::Update(void)
{
}

void Planet::Draw(void)
{
    MV1DrawModel(mTransform.modelId);
}

void Planet::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
	mTransform.Release();
}

void Planet::SetPosition(VECTOR pos)
{
    mTransform.pos = pos;
    mTransform.Update();
}

void Planet::SetRotation(Quaternion rot)
{
	mTransform.quaRot = rot;
	mTransform.Update();
}

float Planet::GetGravityPow(void)
{
	return mGravityPow;
}

void Planet::SetGravityPow(float pow)
{
	mGravityPow = pow;
}

float Planet::GetGravityRadius(void)
{
	return mGravityRadius;
}

void Planet::SetGravityRadius(float radius)
{
	mGravityRadius = radius;
}

Planet::TYPE Planet::GetType(void)
{
	return mType;
}

Transform* Planet::GetTransform(void)
{
	return &mTransform;
}

bool Planet::InRangeGravity(VECTOR pos)
{
	// 球体と点の衝突判定
	// 点 pos
	// 球体 mTransform.pos
	// 半径 mGravityRadius
	auto diff = VSub(pos, mTransform.pos);
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis > (mGravityRadius * mGravityRadius))
	{
		return false;
	}
	return true;
}

bool Planet::InRangeDead(VECTOR pos)
{
	VECTOR diff = VSub(pos, mTransform.pos);
	float dot = VDot(mGravityManager->GetDirGravity(), diff);
	switch (mType)
	{
	case Planet::TYPE::GROUND:
		//if (dot >= 0.0f)
		//{
		//	// 距離判定
		//	if (true)
		//	{
		//		return true;
		//	}
		//}
		break;
	case Planet::TYPE::SPHERE:
		// プレイヤー座標と惑星の中心座標との距離が短ければゲームオーバー
		// 距離 : mDeadLength
		break;
	case Planet::TYPE::TRANS_ROT:
		break;
	case Planet::TYPE::TRANS_CAMERA_FIXED:
		break;
	case Planet::TYPE::ROAD:
		break;
	default:
		break;
	}

	return false;
}

void Planet::SetDeadLength(float len)
{
	mDeadLength = len;
}

Stage::NAME Planet::GetName(void)
{
	return mName;
}
