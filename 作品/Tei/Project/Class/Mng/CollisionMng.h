#pragma once
#include <memory>
#include <list>

class Collision;

#define lpCollisionMng CollisionMng::GetInstance()

class CollisionMng
{
public:
	static CollisionMng& GetInstance()
	{
		static CollisionMng s_instance_;
		return s_instance_;
	}
	void RegistrationCol(std::shared_ptr<Collision> col);
	void Update(void);
private:
	CollisionMng();
	~CollisionMng() = default;

	std::list<std::shared_ptr<Collision>> colList_;
};

