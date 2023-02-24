#pragma once
#include "../Obj.h"

class GmkEvent;

class GimicObj :
	public Obj
{
public:
	GimicObj(std::weak_ptr< GmkEvent>& gmkEvent, const Math::Vector2& pos);
	virtual ~GimicObj();
protected:
	// �M�~�b�N�C�x���g
	std::weak_ptr< GmkEvent> gmkEvent_;
};

