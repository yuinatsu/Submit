#pragma once
#include "../Common//Quaternion.h"
#include "Stage.h"
class SceneManager;
class GravityManager;
class WarpStar;

class Planet
{

public:

	// �d�͂̋���
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// �d�͂�������͈�
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// �Q�[���I�[�o�[�͈�
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	Planet(SceneManager* manager, Stage::NAME name, TYPE type, Transform transform);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	
	// �f�����
	TYPE GetType(void);

	// �f����
	Stage::NAME GetName(void);

	// �ʒu�̐ݒ�
	void SetPosition(VECTOR pos);

	// ��]�̐ݒ�
	void SetRotation(Quaternion rot);

	// �d�͂̋����̎擾�E�ݒ�
	float GetGravityPow(void);
	void SetGravityPow(float pow);
	
	// �d�͔͈͂̎擾�E�ݒ�
	float GetGravityRadius(void);
	void SetGravityRadius(float radius);

	Transform* GetTransform(void);

	// �d�͔͈͓�
	bool InRangeGravity(VECTOR pos);

	// �Q�[���I�[�o�[�͈͓�
	bool InRangeDead(VECTOR pos);

	// �Q�[���I�[�o�[�͈�
	void SetDeadLength(float len);

private:

	SceneManager* mSceneManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// �f�����
	TYPE mType;

	// �f����
	Stage::NAME mName;

	// �d�͂̋���
	float mGravityPow;

	// �d�͔͈�
	float mGravityRadius;

	// �Q�[���[�I�[�o�[����
	float mDeadLength;

};
