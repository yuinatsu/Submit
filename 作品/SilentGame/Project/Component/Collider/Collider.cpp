#include "Collider.h"


Collider::Collider(CheckType myType,const GetHitFunc& getHitFunc, Obj& owner) :
	getHitFunc_{getHitFunc}, Component{owner}, hitFlag_{false}, oldHitFlag_{false}, myType_{myType}
{
}

Collider::~Collider()
{
}

// ID���擾����
ComponentID Collider::GetID() const
{
	// ID��Ԃ�
	return ComponentID::Collider;
}


