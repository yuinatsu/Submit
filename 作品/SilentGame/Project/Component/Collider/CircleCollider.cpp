#include "CircleCollider.h"
#include "FanCollider.h"
#include "../../Object/Obj.h"
#include <DxLib.h>

CircleCollider::CircleCollider(float r, CheckType myType, const GetHitFunc& getHitFunc,Obj& obj) :
	r_{r}, Collider{ myType, getHitFunc,obj}
{
	
}

CircleCollider::~CircleCollider()
{
}

const float CircleCollider::GetRadius(void) const
{
	return r_;
}

void CircleCollider::SetRadius(float r)
{
	r_ = r;
}

void CircleCollider::HitCheck(Collider& col)
{
	HitFlagUpdate();
	col.HitFlagUpdate();
	if (col.IsTargetType(myType_) && IsTargetType(col.GetMyCheckType()))
	{
		if (col.HitCheck(*this))
		{
			// “–‚½‚Á‚½Žž
			getHitFunc_(col);
			col.GetFunc()(*this);
			hitFlag_ = true;
			col.Hit();
		}
	}
}

bool CircleCollider::HitBoolCheck(Collider& col)
{
	bool isHit = false;
	HitFlagUpdate();
	col.HitFlagUpdate();
	if (col.HitCheck(*this))
	{
		// “–‚½‚Á‚½Žž
		getHitFunc_(col);
		col.GetFunc()(*this);
		hitFlag_ = true;
		col.Hit();
		isHit = true;
	}
	return isHit;
}

bool CircleCollider::HitExCheck(Collider& col)
{
	if (col.HitExCheck(*this))
	{
		// “–‚½‚Á‚½Žž
		getHitFunc_(col);
		col.GetFunc()(*this);
		hitFlag_ = true;
		col.Hit();
		return true;
	}
	return false;
}

bool CircleCollider::HitCheck(CircleCollider& col)
{
	// “–‚½‚Á‚Ä‚¢‚é‚©”»’è
	const auto distance = (col.owner_.GetPos() - owner_.GetPos()).SqMagnitude();
	auto r = Math::Square(col.r_ + r_);
	if (distance < r)
	{
		
		return true;
	}
	return false;
}

bool CircleCollider::HitExCheck(CircleCollider& col)
{
	// “–‚½‚Á‚Ä‚¢‚é‚©”»’è
	Math::Vector2 pos = col.owner_.GetPos();
	auto N = col.owner_.GetPos() - owner_.GetPos();
	const auto distance = N.SqMagnitude();
	auto r = Math::Square(col.r_ + r_);
	if (distance < r)
	{
		auto overlap = col.r_ + r_ - N.Magnitude();
		N.Normalize();
		pos += N * overlap * Math::Vector2(0.5,0.5);
		col.owner_.SetPos(pos);
		return true;
	}
	return false;
}

bool CircleCollider::HitCheck(FanCollider& col)
{
	return col.HitCheck(col.GetOwner().GetPos());
}

#ifdef _DEBUG

void CircleCollider::DbgDraw(const Math::Vector2& offset) const
{
	DrawCircle(static_cast<int>(owner_.GetPos().x + offset.x), static_cast<int>(owner_.GetPos().y + offset.y), static_cast<int>(r_), 0xffffff, false);
}

#endif
