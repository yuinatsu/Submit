#include "../../Component/Collider/CircleCollider.h"
#include "../../Component/Animation.h"
#include "../../common/AnimMng.h"
#include "../Unit/Enemy.h"
#include "Clock.h"
#include "../../Scene/SceneMng.h"
#include "../../common/SoundPross.h"
#include "../../common/Debug.h"
#include "../ObjManager.h"

// �{�̂̓����蔻���index
constexpr int bodyCol{ 0 };

// ���̓����蔻���index
constexpr int soundCol{ bodyCol + 1 };

// �A���[�����J�n�����܂ł̎���
constexpr float alarmStartTime{ 5.0f };

// �A���[�����I������܂ł̎���
constexpr float alarmEndTime{ 5.0f };


// �A���[�����̍ő唼�a
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
	// ����CheckType��Decoy�ɂȂ��Ă��邪��X�ǂ��ɂ�����
	bool rtn{ AddComponent(
		std::make_unique<CircleCollider>(
			15.0f,
			CheckType::Decoy,
			[this](Collider& other) {
				SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::clock); },
			*this
					)) };

	// ���̓����蔻���ǉ�
	rtn |= AddComponent(std::make_unique<CircleCollider>(
		1.0f,
		CheckType::Sound,

		[this](Collider& other) {},
		*this)
	);

	// �{�̂̓����蔻��͓G�̖{�̂Ɠ�����悤��
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider, bodyCol)).SetTargetType(static_cast<int>(CheckType::EnemyBody));

	// �G�̉��p�̔���Ɠ�����p��
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
		// ��莞�Ԍo�����特��炵������ς���
		// ��őf�ޓ��ꂽ�炿���Ƃ���
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::clock,SoundProcess::GetSEVolumeEntire(), false);
		stepTime_ = 0.0f;

		// �G�t�F�N�g���o��
		objMng_.AddEffect(EffectName::Alarm, pos_);

		// �R���C�_�[�擾
		auto& col = dynamic_cast<CircleCollider&>(GetComponent(ComponentID::Collider, soundCol));

		// �G�̉��̒T�m�͈͂Ɠ�����悤��
		col.SetTargetType(static_cast<int>(CheckType::EnemySound));

		// ������͈͂�傫������
		col.SetRadius(radiusMax);		updateFunc_ = std::bind(&Clock::AlarmUpdate, this);

		isAlarm_ = true;
	}

}

void Clock::AlarmUpdate(void)
{
	// �����_���ɗh�炷
	randOffset_.x = lpSceneMng.GetRandom().GetRandom(-1.0f, 1.0f);
	randOffset_.y = lpSceneMng.GetRandom().GetRandom(-1.0f, 1.0f);

	if (stepTime_ >= alarmEndTime)
	{
		// ��莞�Ԍo���������
		Destroy();
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::clock);
	}
}
