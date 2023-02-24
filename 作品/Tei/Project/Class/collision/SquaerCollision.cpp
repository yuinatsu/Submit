#include <cassert>
#include <array>
#include "SquaerCollision.h"
#include "../Object/Object.h"
#include "../../_debug/_DebugDispOut.h"

SquaerCollision::SquaerCollision(const Vector2Flt& size, const Vector2Flt offset) :
	Collision(offset),size_(size)
{
}

COLLISION_TAG SquaerCollision::CollisionTag(void)
{
	return COLLISION_TAG::Square;
}

bool SquaerCollision::isHit(std::weak_ptr<Collision> col)
{
	auto CheckHit = [](const Vector2Flt& veca, const Vector2Flt& vecb, const Vector2Flt& point) {
		float tmp = Cross(Vector2Flt(vecb - veca), Vector2Flt(point - veca));
		if (tmp >= 0)
		{
			return true;
		}
		return false;
	};
	//assert(col.lock() && "あたり判定をするコライダーがnullptr");
	auto opos = owner_.lock()->pos_ - offset_;
	//_dbgDrawBox(opos.x, opos.y, opos.x + size_.x, opos.y + size_.y, 0xffff00, false);
	// 矩形VS矩形
	auto scol = std::dynamic_pointer_cast<SquaerCollision>(col.lock());
	if (scol != nullptr)
	{
		//assert((owner_.lock()) && "owner_がnullptr");

		if (col.lock()->CheckOwner(owner_))
		{
			// 同じownerのCollisionはぶつからない
			return false;
		}

		const auto myVertex = GetVertex();
		const auto sVertex = scol->GetVertex();

		for (const auto& vertex : sVertex)
		{
			bool check = true;
			check &= CheckHit(myVertex[0], myVertex[1], vertex);
			check &= CheckHit(myVertex[1], myVertex[2], vertex);
			check &= CheckHit(myVertex[2], myVertex[3], vertex);
			check &= CheckHit(myVertex[3], myVertex[0], vertex);
			if (check)
			{
				return true;
			}
		}
		for (const auto& vertex : myVertex)
		{
			bool check = true;
			check &= CheckHit(sVertex[0], sVertex[1], vertex);
			check &= CheckHit(sVertex[1], sVertex[2], vertex);
			check &= CheckHit(sVertex[2], sVertex[3], vertex);
			check &= CheckHit(sVertex[3], sVertex[0], vertex);
			if (check)
			{
				return true;
			}
		}
	}
	return false;
}

const Rect SquaerCollision::GetRect(void)
{
	if (!owner_.lock())
	{
		return Rect();
	}
	return Rect{ owner_.lock()->pos_, size_ };
}

const std::vector<Vector2Flt> SquaerCollision::GetVertex(void)
{
	std::vector<Vector2Flt> vec;
	if (!owner_.lock())
	{
		//assert(!"SetVertexでowner_がnull");
		return vec;
	}
	const auto& pos = owner_.lock()->pos_ - offset_;
	vec.emplace_back(Vector2Flt{ pos.x,pos.y });
	vec.emplace_back(Vector2Flt{ pos.x + size_.x,pos.y });
	vec.emplace_back(Vector2Flt{ pos.x + size_.x,pos.y + size_.y });
	vec.emplace_back(Vector2Flt{ pos.x ,pos.y + size_.y });
	return vec;
}

void SquaerCollision::SetSize(const Vector2Flt& size)
{
	size_ = size;
}

