#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class SceneManager;
class ResourceManager;
class GravityManager;
class AnimationController;
class Capsule;
class Collider;

class Player
{

public:

	static constexpr float SPEED_MOVE = 5.0f;

	static constexpr float SPEED_RUN = 10.0f;

	static constexpr float TIME_ROT = 1.0f;

	static constexpr float JUMP_POW = 35.0f;

	static constexpr float JUMP_TIME_IN = 0.5f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	Player(SceneManager* manager);
	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);

	void Release(void);

	// ����
	void ProcessMove(void);
	void ProcessJump(void);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �X�΂̌v�Z
	void CalcSlope(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	void AddCollider(Collider* collider);
	void ClearCollider(void);

	Transform* GetTransform(void);

	// ��ԕʍX�V�X�e�b�v
	void UpdateWarpReserve(void);
	void UpdateWarpMove(void);
	Capsule* GetCapsule(void);

	// ��Ԕ���
	bool IsPlay(void);
	bool IsWarpMove(void);
	bool IsAlive(void);

	// ���[�v�J�n
	void StartWarp(float time, Quaternion goalRot, VECTOR goalPos);

	// �Q�[���I�[�o�[���
	void StartDead(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	// ���
	STATE mState;

	// �ړ��X�s�[�h
	float mSpeed;

	// �ړ�����
	VECTOR mMoveDir;

	// �ړ���
	VECTOR mMovePow;

	// �ړ���̍��W
	VECTOR mMovedPos;

	// ��]
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	// �W�����v��
	VECTOR mJumpPow;

	// �W�����v����
	bool mIsJump;

	// �W�����v���͎�t����
	float mStepJump;

	// �Փ˔���ɗp������R���C�_
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	// �Փ˃`�F�b�N
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	// �ۉe
	int mImgShadow;

	// �����G�t�F�N�g
	int mEffectSmoke;
	int mStepFootSmoke;

	// �t���[�����Ƃ̈ړ���
	VECTOR mMoveDiff;

	// ���[�v�p
	float mTimeWarp;
	float mStepWarp;

	// ���[�v�����������̏��
	Quaternion mWarpQua;
	VECTOR mWarpReservePos;

	// ���[�v�����J�n���̃v���C���[���
	Quaternion mReserveStartQua;
	VECTOR mReserveStartPos;

	// ���[�v�O�̘f����
	Stage::NAME mPreWarpName;

	// �O�ՃG�t�F�N�g
	int mEffectWarpOrbit;
	int mHandleWarpOrbitL;
	int mHandleWarpOrbitR;

	// ���f���p�[�c�̃t���[���ԍ�
	int mFrameLeftHand;
	int mFrameRightHand;

	// �X�Ί֌W
	VECTOR mSlopeDir;
	float mSlopeAngleDeg;
	VECTOR mSlopePow;
	VECTOR mHitNormal;
	VECTOR mHitPos;

	// ��ԑJ��
	void ChangeState(STATE state);

	// �Փ˔���
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	// �����G�t�F�N�g
	void EffectFootSmoke(void);

	// �O�ՃG�t�F�N�g
	void EffectWarpOrbit(void);
	void SyncWarpOrbitPos(void);
	void StopOrbitEffect(void);
};

