#include <cmath>
#include "Bullet.h"
#include "SceneManager.h"

Bullet::Bullet(SceneManager* sceneManager)
{
}

void Bullet::Init(VECTOR pos, VECTOR angle)
{
	pos_ = pos;
	angles_ = angle;
}

void Bullet::Update(void)
{
	pos_ = VAdd(pos_, VGet(sinf(angles_.y) * BULLET_SPEED, 0.0f, cosf(angles_.y) * BULLET_SPEED));
}

void Bullet::Draw(void)
{
	DrawSphere3D(pos_, BULLET_RAD, 16, 0xff0000, 0xffffff, true);
}

void Bullet::Release(void)
{
}
