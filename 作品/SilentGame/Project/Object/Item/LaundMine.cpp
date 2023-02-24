#include <DxLib.h>
#include "../../common/AnimMng.h"
#include "../../common/Debug.h"
#include "../../Component/Collider/CircleCollider.h"
#include "LaundMine.h"



LaundMine::LaundMine(const Math::Vector2& pos) :
	Obj{pos}
{
	Init();
}

LaundMine::~LaundMine()
{
}

void LaundMine::Init(void)
{
	AddComponent(std::make_unique<Animation>("Resource/Xml/DefeatItemAnim.xml", "DefeatItem", "LaundMine", *this));
	auto& anim = dynamic_cast<Animation&>(GetComponent(ComponentID::Animator));

	AddComponent(std::make_unique<CircleCollider>(
		10.0f,
		CheckType::LandMine,
		[](Collider& other) {
		},
		*this)
	);

	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(static_cast<int>(CheckType::EnemyBody));
}

void LaundMine::Update(float delta)
{
}

void LaundMine::Draw(const Math::Vector2& drawffset, DrawMng& drawMng)
{
	if (alive_ == true)
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng,pos_ + drawffset, false);
	}
}
