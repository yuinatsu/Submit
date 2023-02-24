#pragma once
#include <memory>
#include <vector>
#include "../../common/Math.h"
#include "PatrolBaseNavi.h"


class TmxObj;

// 巡回用クラス
class PatrolNavi : 
	public PatrolBaseNavi
{
	using Vector2lVec = std::vector<Math::Vector2I>;
	using Vector2lVecItr = Vector2lVec::iterator;
public:
	PatrolNavi(Vector2lVec& route,std::shared_ptr<TmxObj>& tmxObj, Obj& owner);


private:
	const Math::Vector2 GetMovePos(void) const final;

	const Math::Vector2I& GetPos(void)const final;

	void Update(float delta) override;

	// 巡回のマス目
	Vector2lVecItr patrolPos_;

};

