#include "EventShot.h"
#include "SpriteAnimator.h"
#include "SceneManager.h"
#include "Camera.h"

EventShot::EventShot(SceneManager* manager, Transform* transform) :
	ShotBase(manager, transform)
{
}

void EventShot::Update(void)
{
	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	if (!IsAlive())
	{
		return;
	}

	// �ړ�����
	Move();

	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (pos2D.z <= 0 || pos2D.z >= 1)
	{
		VECTOR velocity = VScale(VScale(mDir, -1),100);
		mTransform.pos = VAdd(mTransform.pos, velocity);
		mTransform.Update();
		// ���e�G�t�F�N�g
		CreateExplosion();
		// �J�����V�F�C�N�^�C�~���O
		mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::SHAKE);
	}

	// ���f������̊�{���X�V
	mTransform.Update();


}

float EventShot::GetSpeed(void)
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void)
{
	return 10.0f;
}
