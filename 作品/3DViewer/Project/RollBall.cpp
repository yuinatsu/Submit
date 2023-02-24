#include "RollBall.h"
#include "Unit.h"

RollBall::RollBall(SceneManager* manager, Unit* unit)
{
	sceneMng_ = manager;
	unit_ = unit;
}

void RollBall::Init()
{
}

void RollBall::Update()
{
	step_ += sceneMng_->GetDeltaTime();

	// âÒì]é≤ÇÃç¿ïW
	VECTOR uPos = unit_->GetPos();

	// çÇÇ≥
	pos_.y = 50.0f;

	// îºåa
	float dis = 100.0f;
	pos_.x = uPos.x + (dis * sin(step_));
	pos_.z = uPos.z + (dis * cos(step_));
}

void RollBall::Draw()
{
	DrawSphere3D(pos_, 10.0f, 5, 0xff0000, 0xff0000, true);
}

void RollBall::Release(void)
{
}

VECTOR RollBall::GetPos(void)
{
	return pos_;
}
