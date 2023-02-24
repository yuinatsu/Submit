#include "PatrolBaseNavi.h"

PatrolBaseNavi::PatrolBaseNavi(Vector2lVec& route, std::shared_ptr<TmxObj>& tmxObj, Obj& owner) :
	Component{owner}, route_{route}, tmxObj_{tmxObj}
{
	stopFlag_ = true;
}

PatrolBaseNavi::~PatrolBaseNavi()
{
}
