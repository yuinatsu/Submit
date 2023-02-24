#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"
class SceneManager;
class Planet;
class Player;

class GravityManager
{

public:

	// �d�͕��������S�ɕς��܂ł̎���
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// ��]��
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	enum class STATE
	{
		IDLE,
		STAGE_CHANGE
	};

	GravityManager(SceneManager* manager);

	void Init(void);
	void Update(void);
	void UpdateIdle(void);
	void UpdateChangeStage(void);

	void Calculate(void);

	Planet* GetActivePlanet(void);
	void ChangeActivePlanet(Planet* planet);

	void SetPlayer(Player* player);

	Transform* GetTransform(void);
	
	// �d�͕������擾
	VECTOR GetDirGravity(void);

	// �d�͕����̔��Ε������擾
	VECTOR GetDirUpGravity(void);

	// �d�͂̋������擾
	float GetPower(void);

	// �f���؂�ւ��O�̉�]���
	Quaternion GetPremQuaRot(void);

	// �v���C���[�Ƒ����Փ˂����@��
	void SetLastHitNormal(VECTOR normal);

	// �v���C���[�Ƒ����Փ˂����ʒu
	void SetLastHitPos(VECTOR pos);
	VECTOR GetLastHitPos(void);

protected:

	SceneManager* mSceneManager;

	Player* mPlayer;
	Planet* mActivePlanet;

	Transform mTransform;

	// ���
	STATE mState;

	// �d�͕���
	VECTOR mDirGravity;
	Quaternion mQuaGravity;
	VECTOR mPreDirGravity;

	// �d�͕����̔��Ε���
	VECTOR mDirUpGravity;
	Quaternion mQuaUpGravity;

	// �f���؂�ւ��O�̉�]���
	Quaternion mPreQuaRot;

	// �v���C���[�Ƒ����Փ˂����@��
	VECTOR mLastHitNormal;

	// �v���C���[�Ƒ����Փ˂����ʒu
	VECTOR mLastHitPos;

	float mStep;

	// ��]��
	double mSlerpPow;

	// ��ԑJ��
	void ChangeState(STATE state);

	// �d�͕������v�Z
	VECTOR CalcDirGravity(void);

};

