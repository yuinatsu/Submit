#pragma once
#include "DxLib.h"
#include "Quaternion.h"
#include "Transform.h"
class SceneManager;
class SpriteAnimator;

class ShotBase
{

public:

	// �R���X�g���N�^�EInit�ECreate�̋�ʂ���₱�����̂ŁA
	// Init����
	ShotBase(SceneManager* manager, Transform* parent);

	virtual void Create(VECTOR birthPos, VECTOR dir);
	virtual void Update(void);
	virtual bool CheckAlive(void);
	virtual void Move(void);
	virtual void Draw(void);
	virtual void Release(void);
	virtual void CreateExplosion(void);

	virtual float GetSpeed(void);
	virtual float GetTimeAlive(void);

	bool IsAlive(void);
	void Dead(void);
	VECTOR GetPos(void);
	float GetCollisionRadius(void);

protected:

	SceneManager* mSceneManager;
	SpriteAnimator* mExplosion;

	Transform* mTransformParent;

	// ���f������̊�{���
	Transform mTransform;

	// ���˕���
	VECTOR mDir;

	// ��������
	bool mIsAlive;
	float mStepAlive;

	// �����蔻��̔��a
	float mCollisionRadius;

private:

	static constexpr float DEFAULT_SPEED = 35.0f;
	static constexpr float DEFAULT_TIME_ALIVE = 10.0f;

};

