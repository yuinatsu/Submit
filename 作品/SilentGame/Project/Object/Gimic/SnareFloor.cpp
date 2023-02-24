#include "SnareFloor.h"
#include "../ObjManager.h"
#include "../../common/DrawMng.h"
#include "../../common/ImageMng.h"
#include "../../Component/Collider/CircleCollider.h"
#include "../../common/Structure.h"
#include "../../common/GmkEvent.h"
#include "../../common/SoundPross.h"

constexpr char Floor[]{"./Resource/Image/SnareFloor.png"};

SnareFloor::SnareFloor(std::weak_ptr<GmkEvent>& gmkEvent,const Math::Vector2& pos, ObjManager& objMng, int Type):
	objMng_(objMng), GimicObj{gmkEvent,pos}
{
	type_ = Type;
	//squar_ = std::make_unique<Squar>(pos + Math::Vector2{ -16,-16 }, pos + Math::Vector2{ 16,16 });
	Init();
}

SnareFloor::~SnareFloor()
{
}

void SnareFloor::Init()
{
	flag_ = false;
	AddComponent(std::make_unique<CircleCollider>(
		10.0f,
		CheckType::SnareFloor,
		[this](Collider& other){
			if (!flag_)
			{
				if (!gmkEvent_.expired())
				{
					gmkEvent_.lock()->StartEvent(other.GetOwner().GetPos(), EventType::Alert);
					flag_ = true;
				}
			}
		},
		*this));

	// 当たり判定をプレイヤーの本体のだけに
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(static_cast<int>(CheckType::PlayerBody));
	sTime_ = 0.0f;
}

void SnareFloor::Update(float delta)
{
	if (type_ == 0)
	{
		UpdataAlerm(delta);
	}
	if (type_ == 1)
	{
		UpdataSound(delta);
	}
}

void SnareFloor::UpdataAlerm(float delta)
{
	// squar_->Update(objMng_.GetPlayer().lock()->GetPos());
	/*if (!flag_)
	{
		flag_ = squar_->flag_;
	}*/
	if (flag_ && sTime_ <= 4)
	{
		if (sTime_ == 0.0f)
		{
			AFlag_ = true;
			// sPos_ = objMng_.GetPlayer().lock()->GetPos();
		}
		else
		{
			// sPos_ = (0.0f, 0.0f);
			AFlag_ = false;
		}
		sTime_ += static_cast<float>(delta);
	}
	else
	{
		sTime_ = 0.0f;
		flag_ = false;
	}
}

void SnareFloor::UpdataSound(float delta)
{
	//squar_->Update(objMng_.GetPlayer().lock()->GetPos());
	//AFlag_ = squar_->flag_;
}

void SnareFloor::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	drawMng.Draw(pos_ + offset, 0.0f, lpImageMng.GetID(Floor)[0]);
}

Math::Vector2 SnareFloor::GetApos(void)
{
	return sPos_;
}

bool SnareFloor::GetAFlag(void)
{
	return AFlag_;
}

bool SnareFloor::GetSFlag(void)
{
	return isBeforGameScene_;
}
