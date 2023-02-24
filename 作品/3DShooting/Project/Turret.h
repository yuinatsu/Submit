#pragma once
#include <vector>
#include "Transform.h"
class SceneManager;
class SpriteAnimator;
class TurretShot;

class Turret
{

public:

	static constexpr float SCALE = 80.0f;

	// �Փ˔���F���̔��a
	static constexpr float COLLISION_RADIUS = 200.0f;

	// ��_���[�W�G�t�F�N�g
	static constexpr float TIME_DAMAGED_EFFECT = 2.0f;

	// �C�䃍�[�J����]�ғ��p(deg)
	static constexpr float ANGLE_Y_MIN_STAND = -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = 30.0f;

	// �C�䃍�[�J���ғ���(deg)
	static constexpr float ANGLE_POW_STAND = 0.2f;

	// �C�g���[�J����]�ғ��p(deg)
	static constexpr float ANGLE_X_MIN_GUN = -10.0f;
	static constexpr float ANGLE_X_MAX_GUN = 20.0f;

	// �C�g���[�J���ғ���(deg)
	static constexpr float ANGLE_POW_GUN = 0.2f;

	static constexpr float ATTACK_TIME = 0.15f;

	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	Turret(SceneManager* manager,
		Transform* transformPlayer,
		Transform* transformParent,
		VECTOR localPos,
		VECTOR localAddAxis);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SyncParent(Transform* transform, VECTOR addAxis);

	const std::vector<TurretShot*> GetShots(void)const;

	bool IsAlive(void);
	VECTOR GetPos(void);
	void Damage(void);

private:

	SceneManager* mSceneManager;

	// ���@
	Transform* mTransformPlayer;

	// �{�X
	Transform* mTransformParent;

	// ���f������̊�{���(�C��)
	Transform mTransformStand;

	// ���f������̊�{���(�C�g)
	Transform mTransformGun;

	// ���g�̔���
	SpriteAnimator* mExplosion;

	// ���
	STATE mState;

	// ��͂���̑��΍��W
	VECTOR mLocalPos;

	// �ǉ��̃��[�J����]
	VECTOR mLocalAddAxis;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisStand;

	// �X�ɒǉ��̖C�g���[�J����]
	VECTOR mLocalAddAxisGun;

	// �ϋv��
	int mHp;

	// ��_���[�W�G�t�F�N�g
	float mStepDamaged;

	// �C�䃍�[�J���ғ���(deg)
	float mAnglePowStand;

	// �C�g���[�J���ғ���(deg)
	float mAnglePowGun;

	float mAttackTime;

	std::vector<TurretShot*> shots_;

	// ��ԑJ��
	void ChangeState(STATE state);

};
