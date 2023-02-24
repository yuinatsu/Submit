#include "../../Scene/SceneMng.h"
#include "../../Object/Obj.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "RandomNavi.h"

RandomNavi::RandomNavi(Vector2lVec& route, std::shared_ptr<TmxObj>& tmxObj, Obj& owner) :
	PatrolBaseNavi{route,tmxObj,owner}
{
}

const Math::Vector2 RandomNavi::GetMovePos(void) const
{
	return static_cast<Math::Vector2>(pos_ * tmxObj_->GetTileSize() + tmxObj_->GetTileSize() / 2);
}

const Math::Vector2I& RandomNavi::GetPos(void) const
{
	return pos_;
}

void RandomNavi::Update(float delta)
{
	if (stopFlag_)
	{
		return;
	}

	auto& tileSize = tmxObj_->GetTileSize();
	const auto&& pos = static_cast<Math::Vector2I>(owner_.GetPos()) / tileSize;
	if (pos == pos_)
	{
		pos_ = route_[lpSceneMng.GetRandom().GetRandom(0, static_cast<int>(route_.size()) - 1)];
	}
}
