#include <algorithm>
#include <DxLib.h>
#include "TraceModeCamera.h"
#include "../../../Scene/SceneMng.h"
#include "../Camera.h"
#include "../../../Object/Obj.h"
#include "MoveEventModeCamera.h"


MoveEventModeCamera::MoveEventModeCamera(float allTime, float waitTime,const std::weak_ptr<Obj>& start,const std::weak_ptr<Obj>& end) :
	allTime_{allTime}, waitTime_{waitTime}, start_{start}, end_{end}
{
}

MoveEventModeCamera::~MoveEventModeCamera()
{
}

void MoveEventModeCamera::Update(Camera& camera, float delta)
{
	camera.cameraTime_ += delta;
	// �ړ����̃J�����̏��
	if (eventCamState_ == EventCamState::Move)
	{
		// �^�[�Q�b�g�Ɉړ����̎�

		// ���Ԃ��v�Z
		float now{ camera.cameraTime_ / ((allTime_ - waitTime_) / 2.0f) };

		// �ǐՂ���I�u�W�F�N�g�ƃ^�[�Q�b�g�������Ă��邩�H
		if (!start_.expired() && !end_.expired())
		{
			// ���������Ă���Ƃ����Ԓl���g���^�[�Q�b�g�̕��ֈړ�����
			Math::Vector2 clampPos{ Math::Clamp(start_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom)};
			camera.pos_ = clampPos + ((end_.lock()->GetPos() - clampPos) * now);
		}

		// �l���`�F�b�N
		if (now >= 1.0f)
		{
			// ���K���������Ԓl��1�ȏ�̎��̓^�[�Q�b�g�ɂƂ����Ⴍ������back��
			eventCamState_ = EventCamState::Wait;
			camera.cameraTime_ = 0.0f;
		}
	}
	else if (eventCamState_ == EventCamState::Wait)
	{
		// ���Ԃ��v�Z
		float now{ camera.cameraTime_ / waitTime_ };

		// �l���`�F�b�N
		if (now >= 1.0f)
		{
			// ���K���������Ԓl��1�ȏ�̎��̓^�[�Q�b�g�ɂƂ����Ⴍ������back��
			eventCamState_ = EventCamState::Back;
			camera.cameraTime_ = 0.0f;
		}
	}
	else
	{
		// �ǐՂ���I�u�W�F�N�g�ɖ߂鎞

		// ���Ԃ��v�Z
		float now = (camera.cameraTime_ / ((allTime_ - waitTime_) / 2.0f));

		// �ǐՂ���I�u�W�F�N�g�ƃ^�[�Q�b�g�������Ă��邩�H
		if (!start_.expired() && !end_.expired())
		{
			// �����Ă���Ƃ��^�[�Q�b�g����ǐՂ���I�u�W�F�N�g�֎��Ԓl���g���ړ�����
			Math::Vector2 clampPos{ Math::Clamp(start_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom) };
			camera.pos_ = end_.lock()->GetPos() + ((clampPos - end_.lock()->GetPos()) * now);
		}

		// �l�`�F�b�N
		if (now >= 1.0f)
		{
			// 1�ȏ�̎��J�����̏C���ʒu�ւ̈ړ����J�n����
			eventCamState_ = EventCamState::Move;
			camera.nowMode_ = CamMode::Trace;
			camera.cameraTime_ = 0.0f;
			camera.camMode_ = std::make_unique<TraceModeCamera>(start_);
		}
	}
}

void MoveEventModeCamera::Draw(Camera& camera)
{
	// �㉺�ɍ��F�œ����Șg���o��
	constexpr int frameWidth{ 100 };
	const float timeMax{ ((allTime_ - waitTime_) / 2.0f) };
	float now = (camera.cameraTime_ / timeMax) / (1.0f / 5.0f);
	if (eventCamState_ == EventCamState::Back)
	{
		now = ((timeMax - camera.cameraTime_) / timeMax) / (1.0f / 5.0f);
	}
	else if (eventCamState_ == EventCamState::Wait)
	{
		now = 1.0f;
	}
	now = Math::Clamp(now, 0.0f, 1.0f);
	int frame = static_cast<int>(static_cast<float>(frameWidth) * now);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, lpSceneMng.GetScreenSize().x, frame, 0x000000, true);
	DrawBox(0, lpSceneMng.GetScreenSize().y - frame, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
