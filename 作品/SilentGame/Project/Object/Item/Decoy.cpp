#include "../../Component/Collider/CircleCollider.h"
#include "../../Component/Animation.h"
#include "../../common/AnimMng.h"
#include "Decoy.h"
#include "../../common/Debug.h"

Decoy::Decoy(const Math::Vector2& pos) :
	Obj{pos}
{
	Init();
}

Decoy::~Decoy()
{
}

bool Decoy::Init(void)
{
	bool rtn = AddComponent(
		std::make_unique<CircleCollider>(
			15.0f,
			CheckType::Decoy,
			[this](Collider& other) {
			},
			*this
				));
	AddComponent(std::make_unique<Animation>("Resource/Xml/ItemImage.xml", "Item", "Decoy", *this));
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Decoy");
	return rtn;
}

void Decoy::Update(float delta)
{
	UpdateComponent(delta);
}

void Decoy::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng,pos_ + offset, false);
}
