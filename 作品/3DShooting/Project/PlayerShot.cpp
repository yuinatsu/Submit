#include "PlayerShot.h"
#include "SpriteAnimator.h"
#include "SceneManager.h"

PlayerShot::PlayerShot(SceneManager* manager, Transform* transform):
	ShotBase(manager, transform)
{
}

void PlayerShot::Update(void)
{
	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	if (!IsAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	// ���f������̊�{���X�V
	mTransform.Update();
}


float PlayerShot::GetSpeed(void)
{
	return 20.0f;
}

float PlayerShot::GetTimeAlive(void)
{
	return 10.0f;
}
