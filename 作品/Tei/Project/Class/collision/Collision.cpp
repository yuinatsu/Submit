#include "Collision.h"
#include "../Object/Object.h"

Collision::Collision(const Vector2Flt offset):
	offset_(offset)
{
}

Collision::~Collision()
{
}

void Collision::SetOwner(std::shared_ptr<Object> owner)
{
	owner_ = owner;
}

void Collision::Hit(std::weak_ptr<Object> otehr)
{
	(owner_.lock())->HitCollision(otehr.lock());
}

bool Collision::CheckOwner(std::weak_ptr<Object> owner)
{
	return owner_.lock() == owner.lock();
}

bool Collision::isActiv(void)
{
	if (!(owner_.lock()))
	{
		return false;
	}
	return true;
}

std::weak_ptr<Object> Collision::GetOwner(void)
{
	return owner_;
}

