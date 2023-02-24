#include "../../Component/Animation.h"
#include "../../Component/Navi/PatrolNavi.h"
#include "../../Component/Collider/CircleCollider.h"
#include "Target.h"

Target::Target(std::vector<Math::Vector2I>& route, std::shared_ptr<TmxObj>& tmxObj,const Math::Vector2& pos) :
	Unit{ pos,tmxObj }, isClear_{false}
{
	speed_ = 120.0f;
	spMag_ = 1.0f;
	AddComponent(std::make_unique<Animation>("./Resource/Xml/targetAnim.xml", "target", "Down", *this));
	
	AddComponent(std::make_unique<CircleCollider>(10.0f, CheckType::EnemyBody, [this](Collider& other) {isClear_ = true; }, * this));
	auto& col = dynamic_cast<Collider&>(GetComponent(ComponentID::Collider));
	col.SetTargetType(static_cast<int>(CheckType::PlayerBody));

	AddComponent(std::make_unique<PatrolNavi>(route, tmx_, *this));
	dynamic_cast<PatrolNavi&>(GetComponent(ComponentID::PatrolNavi)).Start();
}

void Target::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, false);
}

void Target::CorrectMoveVecToAnim(void)
{
	auto ckFunc = [this](const Math::Vector2& rV, const Math::Vector2& lV)
	{
		if (Math::Cross(moveVec_, rV) < 0.0f && Math::Cross(moveVec_, lV) > 0.0f)
		{
			return true;
		}
		return false;
	};

	if (ckFunc(Math::downVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::leftVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Down");
		return;
	}

	if (ckFunc(Math::upVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::rightVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Up");
		return;
	}

	if (ckFunc(Math::downVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::leftVector2<float>))
	{

		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Left");
		return;
	}

	if (ckFunc(Math::upVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::rightVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Right");
		return;
	}
}
