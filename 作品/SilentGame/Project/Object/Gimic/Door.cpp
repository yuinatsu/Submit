#include <DxLib.h>
#include "../../Component/Animation.h"
#include "../../Component/Collider/CircleCollider.h"
#include "../../common/DrawMng.h"
#include "../../common/ImageMng.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../../Object/ObjManager.h"
#include "../../common/GmkEvent.h"
#include "Door.h"
#include "../../common/Debug.h"

Door::Door(int myIdx, ObjManager& objMng, std::weak_ptr< GmkEvent>& gmkEvent, std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos) :
	GimicObj{ gmkEvent, pos}, myIdx_{myIdx}, tmxObj_{tmxObj}, objMng_{objMng}, lockFlag_{false}
{
	state_ = State::Close;
	update_ = &Door::Close;
	AddComponent(std::make_unique<Animation>("Resource/Xml/doorAnim.xml", "Door", "Close", *this));
	AddComponent(
		std::make_unique<CircleCollider>(
			30.0f,
			CheckType::Door,
			std::bind(&Door::Hit, this, std::placeholders::_1),
			*this)
	);
	int type = static_cast<int>(CheckType::PlayerBody);
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(type);
	stateTime_ = 0.0f;
	gmkEvent_.lock()->SetCollFunc(std::bind(&Door::StartAlert, this), std::bind(&Door::EndAlert, this));
}

Door::~Door()
{
}

void Door::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	auto& anim = dynamic_cast<Animation&>(GetComponent(ComponentID::Animator));
	anim.Draw(drawMng, pos_ + offset, false);
	//GetComponent(ComponentID::Collider).DbgDraw(offset);
}

void Door::Update(float dleta)
{
	(this->*update_)(dleta);
	stateTime_ += dleta;
	UpdateComponent(dleta);
}

void Door::Close(float delta)
{
	if (lockFlag_)
	{
		SetState(State::Lock);
	}
}

void Door::Open(float dleta)
{
	// ドアの閉まるまでの時間
	constexpr float doorCloseTime{ 3.0f };

	// 経過時間をチェック
	if (stateTime_ >= doorCloseTime)
	{
		// 経過時間が閉まるまでの時間に達してる時閉め始める
		SetState(State::Closing);
	}
}

void Door::Opening(float dleta)
{
	if (dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).IsEnd())
	{
		// アニメーション終了時完全に開いているのでオープンに
		SetState(State::Open);
	}
}

void Door::Closing(float delta)
{
	if (dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).IsEnd())
	{
		// アニメーション終了時完全に閉まっているのでクローズに
		SetState(State::Close);
	}
}

void Door::Lock(float delte)
{
	if (!lockFlag_)
	{
		SetState(State::Close);
	}
}

void Door::Hit(Collider& other)
{
	// 経過時間をリセット
	stateTime_ = 0.0f;
	if (state_ == State::Close )
	{
		if (objMng_.HaveSpecItem(ItemName::Key))
		{
			// クローズの時開け始める
			SetState(State::Opening);
		}
	}
}

void Door::StartAlert(void)
{
	lockFlag_ = true;
}

void Door::EndAlert(void)
{
	lockFlag_ = false;
}

void Door::SetState(Door::State state)
{
	
	stateTime_ = 0.0f;
	switch (state)
	{
	case Door::State::Close:
		update_ = &Door::Close;
		if (state_ != State::Lock)
		{
			myIdx_ = tmxObj_->OnCol(myIdx_);
		}
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Close");
		break;
	case Door::State::Opening:
		update_ = &Door::Opening;
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Opening");
		break;
	case Door::State::Open:
		update_ = &Door::Open;
		myIdx_ = tmxObj_->OffCol(myIdx_);
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Open");
		break;
	case Door::State::Closing:
		update_ = &Door::Closing;
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Closing");
		break;
	case Door::State::Lock:
		update_ = &Door::Lock;
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Lock");
		break;
	default:
		break;
	}
	state_ = state;
}
