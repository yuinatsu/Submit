#include "../../Component/Collider/CircleCollider.h"
#include "../../Component/Animation.h"
#include "../../common/AnimMng.h"
#include "../Unit/Enemy.h"
#include "Alcohol.h"

Alcohol::Alcohol(const Math::Vector2& pos) :
	Obj{pos}
{
	AddComponent(
		std::make_unique<CircleCollider>(
			15.0f,
			CheckType::Alcohol,
			[this](Collider& other) {
			},
				*this
				));

	AddComponent(std::make_unique<Animation>("Resource/Xml/ItemImage.xml", "Item", "Alcohol", *this));

}

Alcohol::~Alcohol()
{
}

bool Alcohol::Init(void)
{
    return false;
}

void Alcohol::Update(float delta)
{
	UpdateComponent(delta);
}

void Alcohol::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng,pos_ + offset, false);
	//(GetComponent(ComponentID::Collider, "BodyCol")).DbgDraw(offset);
}
