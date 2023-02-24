#include "../../Object/Obj.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "PatrolNavi.h"

PatrolNavi::PatrolNavi(Vector2lVec& route,std::shared_ptr<TmxObj>& tmxObj,Obj& owner) :
	PatrolBaseNavi{route,tmxObj,owner}
{
	patrolPos_ = route_.begin();
}

const Math::Vector2 PatrolNavi::GetMovePos(void) const
{
	return static_cast<Math::Vector2>(*patrolPos_ * tmxObj_->GetTileSize() + tmxObj_->GetTileSize() / 2);
}

const Math::Vector2I& PatrolNavi::GetPos(void) const
{
	return *patrolPos_;
}



void PatrolNavi::Update(float delta)
{
	// ストップフラグをチェックする
	if (stopFlag_)
	{
		// ストップの時これ以上処理しない
		return;
	}

	auto& tileSize = tmxObj_->GetTileSize();
	const auto&& pos = static_cast<Math::Vector2I>(owner_.GetPos()) / tileSize;
	if (pos == *patrolPos_)
	{
		++patrolPos_;
		if (patrolPos_ == route_.end())
		{
			patrolPos_ = route_.begin();
		}
	}
}


