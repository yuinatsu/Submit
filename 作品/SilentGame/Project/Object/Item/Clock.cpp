#include "../../Component/Collider/CircleCollider.h"
#include "../../Component/Animation.h"
#include "../../common/AnimMng.h"
#include "../Unit/Enemy.h"
#include "Clock.h"
#include "../../Scene/SceneMng.h"
#include "../../common/SoundPross.h"
#include "../../common/Debug.h"
#include "../ObjManager.h"

// 本体の当たり判定のindex
constexpr int bodyCol{ 0 };

// 音の当たり判定のindex
constexpr int soundCol{ bodyCol + 1 };

// アラームが開始されるまでの時間
constexpr float alarmStartTime{ 5.0f };

// アラームが終了するまでの時間
constexpr float alarmEndTime{ 5.0f };


// アラーム時の最大半径
constexpr float radiusMax{ 200.0f };

Clock::Clock(const Math::Vector2& pos, ObjManager& objMng) :
	stepTime_{0.0f}, objMng_(objMng), Obj{pos}
{
	Init();
}

Clock::~Clock()
{
}

void Clock::StopAlarm(void) const
{
	if (isAlarm_)
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::clock);
	}
}

void Clock::RestartAlarm(void) const
{
	if (isAlarm_)
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::clock, SoundProcess::GetSEVolumeEntire(), false);
	}
}

bool Clock::Init(void)
{
	// 現在CheckTypeがDecoyになっているが後々どうにかする
	bool rtn{ AddComponent(
		std::make_unique<CircleCollider>(
			15.0f,
			CheckType::Decoy,
			[this](Collider& other) {
				SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::clock); },
			*this
					)) };

	// 音の当たり判定を追加
	rtn |= AddComponent(std::make_unique<CircleCollider>(
		1.0f,
		CheckType::Sound,

		[this](Collider& other) {},
		*this)
	);

	// 本体の当たり判定は敵の本体と当たるように
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider, bodyCol)).SetTargetType(static_cast<int>(CheckType::EnemyBody));

	// 敵の音用の判定と当たる用に
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider, soundCol)).SetTargetType(static_cast<int>(CheckType::Non));
	rtn |= AddComponent(std::make_unique<Animation>("Resource/Xml/ItemImage.xml", "Item", "Clock", *this));

	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Clock");

	updateFunc_ = std::bind(&Clock::NonUpdate, this);
	isAlarm_ = false;
	return rtn;
}

void Clock::Update(float delta)
{
	UpdateComponent(delta);
	stepTime_ += delta;
	updateFunc_();
}

void Clock::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + randOffset_ + offset, false);
}

void Clock::NonUpdate(void)
{
	if (stepTime_ >= alarmStartTime)
	{
		// 一定時間経ったら音を鳴らし処理を変える
		// 後で素材入れたらちゃんとかく
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::clock,SoundProcess::GetSEVolumeEntire(), false);
		stepTime_ = 0.0f;

		// エフェクトを出す
		objMng_.AddEffect(EffectName::Alarm, pos_);

		// コライダー取得
		auto& col = dynamic_cast<CircleCollider&>(GetComponent(ComponentID::Collider, soundCol));

		// 敵の音の探知範囲と当たるように
		col.SetTargetType(static_cast<int>(CheckType::EnemySound));

		// 当たる範囲を大きくする
		col.SetRadius(radiusMax);		updateFunc_ = std::bind(&Clock::AlarmUpdate, this);

		isAlarm_ = true;
	}

}

void Clock::AlarmUpdate(void)
{
	// ランダムに揺らす
	randOffset_.x = lpSceneMng.GetRandom().GetRandom(-1.0f, 1.0f);
	randOffset_.y = lpSceneMng.GetRandom().GetRandom(-1.0f, 1.0f);

	if (stepTime_ >= alarmEndTime)
	{
		// 一定時間経ったら消滅
		Destroy();
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::clock);
	}
}
