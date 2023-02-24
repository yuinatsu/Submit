#pragma once
#include <vector>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class Player;

class WarpStar
{

public:

	// ��������
	static constexpr float TIME_WARP_RESERVE = 2.0f;

	// ���a
	static constexpr float RADIUS = 80.0f;

	// �G�t�F�N�g�̈ʒu
	static constexpr VECTOR EFFECT_RELATIVE_POS = { 0.0f, 70.0f, 0.0f };
	
	// ���[�v�O�̏����ړ����΍��W
	static constexpr VECTOR WARP_RELATIVE_POS = { 0.0f, -80.0f, 10.0f };

	// ��]���x
	static constexpr float SPEED_ROT_IDLE = 3.0f;
	static constexpr float SPEED_ROT_RESERVE = 15.0f;

	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		MOVE
	};

	WarpStar(SceneManager* manager, Player* player, Transform transform);
	void Init(void);
	void Update(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateMove(void);
	void Draw(void);
	void Release(void);

	Transform* GetTransform(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	Player* mPlayer;

	Transform mTransform;

	STATE mState;

	// ���[�v����
	Quaternion mWarpQua;
	VECTOR mWarpReservePos;

	void ChangeState(STATE state);

	// ��]
	void RotateZ(float speed);

};

