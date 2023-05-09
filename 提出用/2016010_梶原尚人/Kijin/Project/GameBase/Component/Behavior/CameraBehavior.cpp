#include "CameraBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../../Scene/BaseScene.h"
#include "../../Common/Camera.h"
#include "PlayerBehavior.h"
#include "../../Common/Input/InputConfig/InputConfig.h"
#include "../../Common/Debug.h"


constexpr float offsetHeight{ 200.0f };
constexpr float offsetLength{ 500.0f };

// �J�������V�F�C�N�������
constexpr struct Shake {
	Vector3 dir{ 1.0f,1.0f,0.0f };
	float width{ 4.0f };
	float speed{ 60.0f };
} shake;

// �v���C���[�ړ����̃J�����I�t�Z�b�g
constexpr struct CameraOffset {
	float dodge = 200.0f;
	float dash = 50.0f;
	float walk = 0.0f;
} offset;


void CameraBehavior::StartShake(float shakeTime)
{
	shakeTime_ = shakeTime;
	isShake_ = true;
	defPos_ = transform_->GetPos();
	stepTime_ = 0.0f;
}

void CameraBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stepTime_ += delta;
	(this->*update_)(scene, objectManager, delta, controller);
}

void CameraBehavior::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	if (nowType_ == Type::Sync)
	{
		transform_->Pos() += upVector3<float> *offsetHeight;
		update_ = &CameraBehavior::UpdateSync;
		stepTime_ = 0.0f;
		offset_ = 0.0f;
		cnt_ = 0.0f;
		offsetMax_ = 0.0f;

		auto syncObj = objectManager.GetComponent<Transform>(syncID_);
		if (syncObj.IsActive())
		{
			transform_->Pos() = syncObj->GetPos() + (-transform_->GetForward() * offsetLength);
			transform_->Pos().y = syncObj->GetPos().y + offsetHeight;
			defPos_ = transform_->GetPos();
		}
	}
}

void CameraBehavior::UpdateSync(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// ���_�̑��삩���]�p�x�����߂�
	constexpr auto half = SceneManager::screenSize_<float> / 2.0f;
	auto mousePos = controller.GetRightInput();
	auto move = mousePos.x - half.x;
	float rotY = (move * speed_ * delta);
	// ��]��K������
	transform_->AddRotation({ 0.0f,rotY,0.0f });
	
	// �J�[�\���𒆐S�ɖ߂�
	controller.SetCursorPos();


	// �����ΏۂƓ�������
	auto syncObj = objectManager.GetComponent<Transform>(syncID_);
	if (syncObj.IsActive())
	{
		// ���W�𓯊�������
		float t = Square(20.0f * delta);
		auto y = transform_->Pos().y;
		transform_->Pos() = Learp(transform_->GetPos(), syncObj->GetPos() + (-transform_->GetForward() * offsetLength), t);
		 
		transform_->Pos().y = std::lerp(y, syncObj->GetPos().y + offsetHeight, t);
		defPos_ = transform_->GetPos();
		
		/*transform_->Pos() = syncObj->GetPos() + (-transform_->GetForward() * offsetLength);
		transform_->Pos().y = syncObj->GetPos().y + offsetHeight;*/

		// ���x�ɂ���ăJ������x���p�x��ς���
		constexpr float heightMax = 1000.0f;
		auto h = syncObj->GetPos().y / heightMax;
		auto rad = std::lerp(0.0f, Deg2Rad(15.0f), h);
		transform_->SetRotation({ rad, transform_->GetRotation().ToEuler().y, 0.0f});
	}

	if (isShake_)
	{
		// �V�F�C�N���̏���
		Shake(delta);
	}

	auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
	if (player.IsActive())
	{
		offset_ = (player->IsDodge()) ? offset.dodge
			: (player->IsDash()) ? offset.dash
			: offset.walk;
		if (offset_ != cnt_)
		{
			CameraOffset(delta);
		}
	}
	// �J�����ɓ���������
	scene.GetCamera().SetPos(transform_->GetPos() + (-transform_->GetForward() * cnt_));
	scene.GetCamera().SetRotation(transform_->GetRotation());
}

void CameraBehavior::UpdateNon(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	constexpr auto half = SceneManager::screenSize_<float> / 2.0f;
	auto mousePos = controller.GetRightInput();
	auto move = mousePos.x - half.x;
	float rotY = (move * 0.5f * delta);
	float rotX = (mousePos.y - half.y) * 0.5f * delta;
	// ��]��K������
	transform_->AddRotation({ 0.0f,rotY,0.0f });
	
	
	// �J�[�\���𒆐S�ɖ߂�
	controller.SetCursorPos();

	auto& input = controller.GetLeftInput();
	if (input.SqMagnitude() > 0.0f)
	{
		Vector3 dir{ -input.y,0.0f,-input.x };
		transform_->Pos() += (transform_->GetRotation() * dir) * delta * 20.0f;
	}

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		transform_->Pos().y += delta * 20.0f;
	}
	if (CheckHitKey(KEY_INPUT_LCONTROL))
	{
		transform_->Pos().y -= delta * 20.0f;
	}
	// �J�����ɓ���������
	scene.GetCamera().SetPos(transform_->GetPos());
	scene.GetCamera().SetRotation(transform_->GetRotation());
}

void CameraBehavior::Shake(float delta)
{
	if (stepTime_ >= shakeTime_)
	{
		// �w��̎��Ԍo������h�炷�̂���߂�
		isShake_ = false;
		transform_->Pos() = defPos_;
		stepTime_ = 0.0f;
		return;
	}

	// ���W��sin��shake.dir�����ɗh�炷
	transform_->Pos() += shake.dir * (shake.width * std::sin(stepTime_ * shake.speed));
}

void CameraBehavior::CameraOffset(float delta)
{
	if (offset_ > cnt_)
	{
		cnt_ += delta * 1000.0f;
		offsetMax_ = offset_;
	}
	else if (offset_ < cnt_)
	{
		cnt_ -= delta * 1000.0f;
		if (offset_ >= cnt_)
		{
			offsetMax_ = offset_;
		}
	}
	cnt_ = Clamp(cnt_, 0.0f, offsetMax_);
}

void CameraBehavior::SetSpeed(float speed)
{
	speed_ = (lpConfigMng.GetNowType() == -1 ? speed * 0.25f : speed);
}

void CameraBehavior::SetType(Type type)
{
	nowType_ = type;
	if (type == Type::Sync)
	{
		update_ = &CameraBehavior::UpdateSync;
	}
	else
	{
		update_ = &CameraBehavior::UpdateNon;
	}
}