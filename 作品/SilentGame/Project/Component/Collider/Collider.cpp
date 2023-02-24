#include "Collider.h"


Collider::Collider(CheckType myType,const GetHitFunc& getHitFunc, Obj& owner) :
	getHitFunc_{getHitFunc}, Component{owner}, hitFlag_{false}, oldHitFlag_{false}, myType_{myType}
{
}

Collider::~Collider()
{
}

// ID‚ðŽæ“¾‚·‚é
ComponentID Collider::GetID() const
{
	// ID‚ð•Ô‚·
	return ComponentID::Collider;
}


