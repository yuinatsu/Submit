#pragma once
#include <vector>
#include <memory>
#include "../../common/Math.h"
#include "PatrolBaseNavi.h"


class TmxObj;

// ランダムにルートの点を移動するクラス(仮で)
class RandomNavi :

    public PatrolBaseNavi
{
	using Vector2lVec = std::vector<Math::Vector2I>;
	using Vector2lVecItr = Vector2lVec::iterator;
public:
	RandomNavi(Vector2lVec& route, std::shared_ptr<TmxObj>& tmxObj, Obj& owner);

private:
	const Math::Vector2 GetMovePos(void) const final;

	const Math::Vector2I& GetPos(void)const final;

	void Update(float delta) override;

	Math::Vector2I pos_;
};

