#include "GameCommon.h"
#include "AsoUtility.h"
#include "Quaternion.h"
#include "SceneManager.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "SpriteAnimator.h"
#include "ShotBase.h"

ShotBase::ShotBase(SceneManager* manager, Transform* parent)
{

	// �������n
	mSceneManager = manager;
	mTransformParent = parent;
	mExplosion = new SpriteAnimator(
		manager, ResourceManager::SRC::SHOT_EXPLOSION, 20.0f, 8.0f);

	mIsAlive = false;

	// �����ϓ�����������ΐ�pGetter��
	mCollisionRadius = 10.0f;

	// ���f������̊�{���
	ResourceManager* rem = mSceneManager->GetResource();
	mTransform.modelId = rem->LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	float scale = 0.1f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = mTransformParent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

}

void ShotBase::Create(VECTOR birthPos, VECTOR dir)
{

	// �ė��p�\�Ȃ悤�ɂ���

	// �w������ɒe���΂�
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	// �����n������
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

}

void ShotBase::Update(void)
{

	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	if (!CheckAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	// ���f������̊�{���X�V
	mTransform.Update();

}

bool ShotBase::CheckAlive(void)
{

	// ��������
	mStepAlive -= mSceneManager->GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;

}

void ShotBase::Move(void)
{

	// �ړ�
	VECTOR velocity = VScale(mDir, GetSpeed());
	mTransform.pos = VAdd(mTransform.pos, velocity);

}

void ShotBase::Draw(void)
{

	mExplosion->Draw();

	if (!mIsAlive)
	{
		return;
	}

	MV1DrawModel(mTransform.modelId);

}

void ShotBase::Release(void)
{
	mExplosion->Release();
	delete mExplosion;
	//MV1DeleteModel(mTransform.modelId);
}

float ShotBase::GetSpeed(void)
{
	return DEFAULT_SPEED;
}

float ShotBase::GetTimeAlive(void)
{
	return DEFAULT_TIME_ALIVE;
}

bool ShotBase::IsAlive(void)
{
	return mIsAlive;
}

void ShotBase::Dead(void)
{
	mIsAlive = false;
}

void ShotBase::CreateExplosion(void)
{
	mIsAlive = false;
	mExplosion->Create(mTransform.pos);
}

VECTOR ShotBase::GetPos(void)
{
	return mTransform.pos;
}

float ShotBase::GetCollisionRadius(void)
{
	return mCollisionRadius;
}
