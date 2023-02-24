#include "ButtonBase.h"

ButtonBase::ButtonBase(const Vector2& pos) :
	UiBase{pos}, isHitCursor_{false}
{
}

ButtonBase::~ButtonBase()
{
}
