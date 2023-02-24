#include <algorithm>
#include "CollisionMng.h"
#include "../collision/Collision.h"

void CollisionMng::RegistrationCol(std::shared_ptr<Collision> col)
{
	colList_.emplace_back(col);
}

void CollisionMng::Update(void)
{
	auto itr = std::remove_if(colList_.begin(), colList_.end(), [](std::shared_ptr<Collision> col)->bool {return !col->isActiv(); });
	colList_.erase(itr, colList_.end());
	for (const auto& cola : colList_)
	{
		for (const auto& colb : colList_)
		{
			if (cola->isHit(colb))
			{
				cola->Hit(colb->GetOwner());
			}
		}
	}
}

CollisionMng::CollisionMng()
{
}
