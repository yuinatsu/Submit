#include "../../common/DrawMng.h"
#include "TrapAlarm.h"
#include "../ObjManager.h"
#include "../../common//ImageMng.h"
#include "../../Component/Collider/CircleCollider.h"
#include "../../common/GmkEvent.h"
#include "../../common/SoundPross.h"

constexpr char closeImg[]{ "./Resource/Image/boxAlarm.png" };
constexpr char openImg[]{ "./Resource/Image/boxAlarm1.png" };

TrapAlarm::TrapAlarm(const Math::Vector2& pos,std::weak_ptr< GmkEvent>& gmkEvent):
	GimicObj{gmkEvent,pos}
{
	pos_ = pos;

	// コンポーネントを追加
	AddComponent(std::make_unique<CircleCollider>(
		10.0f,
		CheckType::Box,
		std::bind(&TrapAlarm::Hit,this,std::placeholders::_1),
		*this));

	// プレイヤーの本体の判定とだけ当たるように
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(static_cast<int>(CheckType::PlayerBody));

	Init();
}

TrapAlarm::~TrapAlarm()
{
}

void TrapAlarm::Init(void)
{
	sTime_ = 0.0f;
}

void TrapAlarm::Update(float delta)
{
	if (flag_ && sTime_ <= 4)
	{
		sTime_ += delta;
	}
	else
	{
		sTime_ = 0.0f;
		flag_ = false;
	}
}

void TrapAlarm::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	drawMng.Draw(pos_ + offset, 0.0f, lpImageMng.GetID((flag_ == true ? openImg : closeImg))[0]);
}

Vector2 TrapAlarm::GetSpos(void)
{
	return sPos_;
}

void TrapAlarm::Hit(Collider& othre)
{
	auto tmp = othre.GetOwner().GetPos() - pos_;
	auto length = dynamic_cast<CircleCollider&>(othre).GetRadius() +
		dynamic_cast<CircleCollider&>(GetComponent(ComponentID::Collider)).GetRadius();
	othre.GetOwner().SetPos(pos_ + (tmp.Normalized() * length));
	flag_ = true;
	sPos_ = othre.GetOwner().GetPos();
	if (!gmkEvent_.expired())
	{
		gmkEvent_.lock()->StartEvent(sPos_, EventType::Alert);
	}
}



