#include "GimicObj.h"

GimicObj::GimicObj(std::weak_ptr<GmkEvent>& gmkEvent, const Math::Vector2& pos) :
	gmkEvent_{gmkEvent}, Obj{pos}
{
}

GimicObj::~GimicObj()
{
}
