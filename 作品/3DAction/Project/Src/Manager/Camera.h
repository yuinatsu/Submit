#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
class SceneManager;
class GravityManager;
class Transform;
class Collider;

class Camera
{

public:

	// FIXED_POINT : �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 50.0f, -400.0f };

	// FOLLOW : �����_(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 0.0f, 500.0f };

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = -15.0f * (DX_PI_F / 180.0f);
	
	// �Ǐ]�ΏۂƂ̒������W
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);
	void Release(void);

	void ProcessRot(void);

	void SetTransform(Transform* transform);

	VECTOR GetPos(void);
	VECTOR GetAngles(void);
	VECTOR GetTargetPos(void);
	Quaternion GetQuaRot(void);
	Quaternion GetQuaRotOutX(void);

	VECTOR GetDir(void);

	void ChangeMode(MODE mode);

private:

	SceneManager* mSceneManager;
	GravityManager* mGravityManager;

	// �J�������Ǐ]�ΏۂƂ���Transform
	Transform* mSyncTransform;

	// �J�������[�h
	MODE mMode;

	// �J�����̈ʒu
	VECTOR mPos;

	// �J�����p�x(rad)
	VECTOR mAngles;

	// X����]�������p�x
	Quaternion mQuaRotOutX;

	// �J�����p�x
	Quaternion mQuaRot;

	// �����_
	VECTOR mTargetPos;

	// �J�����̏����
	VECTOR mCameraUp;

	void SetDefault(void);

	// Transform�̈ʒu�Ɠ��������
	void SyncTransform(void);

};

