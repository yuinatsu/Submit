#pragma once
#include <vector>
#include <memory>
#include "../common/Vector2.h"

enum class COLLISION_TAG
{
	Square,
	Circle,
	Triangle
};

class Object;

class Collision
{
public:
	Collision(const Vector2Flt offset);
	virtual ~Collision();

	void SetOwner(std::shared_ptr<Object> owner);

	virtual  COLLISION_TAG CollisionTag(void) = 0;

	virtual bool isHit(std::weak_ptr<Collision> col) = 0;
	void Hit(std::weak_ptr<Object> otehr);

	bool CheckOwner(std::weak_ptr<Object> owner);

	bool isActiv(void);
	std::weak_ptr<Object> GetOwner(void);
protected:
	std::weak_ptr<Object> owner_;
	Vector2Flt offset_;
};

