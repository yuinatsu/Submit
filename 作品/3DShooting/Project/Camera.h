#pragma once
#include <vector>
#include "DxLib.h"
#include "Quaternion.h"

enum class CAMERA_MODE
{
	NONE,
	FREE,
	FIXED,
	FOLLOW,
	FOLLOW_SPRING,
	SHAKE
};

class SceneManager;
class Player;

class Camera
{

public:

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };


	// �J�����ʒu�ƒ����_�Ƃ̑��΍��W
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };

	// Player�ƃJ�����Ƃ̑��΍��W
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW = { 0.0f, 25.0f, -80.0f };
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW_SPRING = { 0.0f, 25.0f, 150.0f };
	
	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void SetDefault(void);

	void Update(void);

	void SetBeforeDraw(void);
	void Draw(void);
	void DrawDebug(void);
	void DrawUI(void);
	void Release(void);

	VECTOR GetPos(void);
	VECTOR GetTargetPos(void);
	VECTOR GetDir(void);

	void ChangeMode(CAMERA_MODE mode);

	void SetPlayer(Player* player);

private:

	SceneManager* mSceneManager;
	Player* player_;

	CAMERA_MODE mMode;
	// �J�����̈ʒu
	VECTOR mPos;
	// �����_
	VECTOR mTargetPos;

	// �J�����̏����
	VECTOR mCameraUp;

	VECTOR mVelocity;
	// �J�����p�x
	Quaternion mQuaRot;

	float stepShake_;
	VECTOR defPos_;
	VECTOR shakeDir_;

	void Move();
	void Rotate();

	void SetBeforeDrawFree();
	void SetBeforeDrawFixed();
	void SetBeforeDrawFollow();
	void SetBeforeDrawSpring();
	void SetBeforeDrawShake();
};

