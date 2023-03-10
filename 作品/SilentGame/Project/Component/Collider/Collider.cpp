#include "Collider.h"


Collider::Collider(CheckType myType,const GetHitFunc& getHitFunc, Obj& owner) :
	getHitFunc_{getHitFunc}, Component{owner}, hitFlag_{false}, oldHitFlag_{false}, myType_{myType}
{
}

Collider::~Collider()
{
}

// IDを取得する
ComponentID Collider::GetID() const
{
	// IDを返す
	return ComponentID::Collider;
}


