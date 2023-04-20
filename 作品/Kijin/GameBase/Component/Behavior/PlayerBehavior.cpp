#include "../../Scene/BaseScene.h"
#include "PlayerBehavior.h"
#include "../../Common/Input/InputType/Keyboard.h"
#include "../../Common/Input/InputType/Pad.h"
#include "../../Common/Debug.h"
#include "../../Common/Math.h"
#include "../../Common/Camera.h"
#include "../../Common/SoundPross.h"
#include "../Collider/CharactorCollider.h"
#include "../Info/ObjectInfo.h"
#include "EnemyBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerSkillBehavior.h"
#include "CameraBehavior.h"

PlayerBehavior::PlayerBehavior(void)
{
	Init();
}

bool PlayerBehavior::Init(void)
{
	// �ړ�
	dodgeTimer_ = 0.0f;
	noDashTime_ = 0.0f;
	isStaminaLoss_ = false;
	// ��]
	angles_ = Vector3{ Deg2Rad(0.0f),Deg2Rad(dir_.up),Deg2Rad(0.0f) };
	// �U��
	atkLimit_ = false;
	isAttack_ = false;
	isDodge_ = false;
	combo_ = 0;
	canSkill_ = false;
	isSkill_ = false;
	// �A�j���[�V����
	anim_.push_back(std::pair(AnimIndex::Idle, animtime_.idle));
	anim_.push_back(std::pair(AnimIndex::MoveSlow, animtime_.move));
	anim_.push_back(std::pair(AnimIndex::ForwardMove, animtime_.move));
	anim_.push_back(std::pair(AnimIndex::Attack1, animtime_.attack1));
	anim_.push_back(std::pair(AnimIndex::Attack2, animtime_.attack2));
	anim_.push_back(std::pair(AnimIndex::Attack3, animtime_.attack3));
	anim_.push_back(std::pair(AnimIndex::Skill, animtime_.skill));

	// �Q�[�W
	gauge_.emplace(UiID::Hp, std::pair(200.0f, std::pair(0.0f, 200.0f)));
	gauge_.emplace(UiID::Skill, std::pair(0.0f, std::pair(0.0f, 200.0f)));
	gauge_.emplace(UiID::Stamina, std::pair(200.0f, std::pair(0.0f, 200.0f)));
	atkCnt_ = 0;
	// �T�E���h
	for (int i = 0; i < static_cast<int>(SOUNDNAME_SE::Max); i++)
	{
		sound_.push_back(std::pair(static_cast<SOUNDNAME_SE>(i), false));
	}
	
	return true;
}

void PlayerBehavior::Begin(ObjectManager& objectManager)
{
	// Y���W�n�ʌŒ�
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	// ���f�����
	auto modelrender = objectManager.GetComponent<ModelRender>(ownerId_);
	auto& handle = modelrender->GetHandle();
	// �A�j���[�V����
	animation_ = objectManager.GetComponent<Animator>(ownerId_);
	for (int i = 0; i <= 10; i++)
	{
		animation_->AddAnimation(i, false);
	}
	// �����A�j���[�V�����Z�b�g
	animation_->SetNextAnimation(static_cast<int>(AnimIndex::Idle));
	// �����蔻��
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&PlayerBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));

	// �G�t�F�N�g
	// �u���X�^�[
	auto blasterID = objectManager.CreateFromFactory(FactoryID::BlasterEffect, ownerId_, transform_->GetPos());
	objectManager.Begin(blasterID);
	blaster_ = objectManager.GetComponent<ThrusterBehavior>(blasterID);
	// ����
	auto sandID = objectManager.CreateFromFactory(FactoryID::SandSmokeEffect, ownerId_, transform_->GetPos());
	objectManager.Begin(sandID);
	sand_ = objectManager.GetComponent<SandSmokeBehavior>(sandID);
	// �����i�L����j�G�t�F�N�g
	auto sandDiffID = objectManager.CreateFromFactory(FactoryID::SandSmokeDiffusionEffect, ownerId_, transform_->GetPos());
	objectManager.Begin(sandDiffID);
	sandDiff_ = objectManager.GetComponent<SandSmokeDiffusionBehavior>(sandDiffID);
	sandDiff_->Play();
	// ����(�ړ��x�����p)
	auto sandSlowID = objectManager.CreateFromFactory(FactoryID::SandSmokeSlowMoveEffect, ownerId_, transform_->GetPos());
	objectManager.Begin(sandSlowID);
	sandSlow_ = objectManager.GetComponent<SandSmokeSlowMoveBehavior>(sandSlowID);
	// �J����
	camera_ = objectManager.GetComponent<Transform>(objectManager.GetCameraID());
	// �n�ʍ��W
	float_.groundPosY = transform_->Pos().y;


	lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerMove,true);
}

void PlayerBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// �n�ʂ̍�������
	CorrectionGroundPos();

	// ���쐧��
	auto speed = ProcessDash(controller, delta);
	collider_->SetSpeed(speed);
	auto rot = ProcessMove(scene, objectManager, controller, speed, delta);
	ProcessRotate(objectManager, scene, controller, rot);
	ProcessJump(controller);
	if (float_.canFloat)
	{
		// ���V
		//ProcessFloat();
	}
	ProcessAttack(controller, objectManager);
	ProcessStamina(controller);
	if (isStaminaLoss_)
	{
		// �N�[���^�C��
		CoolTimer(delta);
	}
	ProcessSkill(controller, objectManager);
	// �A�j���[�V��������
	Animation(controller, objectManager);
	// ���֌W����
	Sound();


}

float PlayerBehavior::ProcessMove(BaseScene& scene, ObjectManager& objectManager, Controller& controller, float speed, float delta)
{
	// �ړ��L�[���͔���
	auto rot = controller.GetLeftInput().GetAngle();
	// �K�E�Z�������͈ړ����̂������Ȃ�
	if (isSkill_)
	{
		ProcessMoveSkill(1.0f, delta);
		return rot;
	}

	// �A�j���[�V�����ړ���
	auto animMove = animation_->GetAnimState()->GetMovePosition();
	transform_->Pos().x += sinf(angles_.y) * animMove.z;
	transform_->Pos().z += cosf(angles_.y) * animMove.z;
	
	// �ʏ�ړ�
	if (controller.GetLeftInput().SqMagnitude() > 0.0f)
	{
		auto camAngle = camera_->GetRotation().ToEuler();
		transform_->Pos().x -= sinf(camAngle.y + rot) * speed;
		transform_->Pos().z -= cosf(camAngle.y + rot) * speed;
		// �ړ����͕��V�s��
		float_.canFloat = false;
		sandDiff_->Play();
	}
	else
	{
		// ���V��
		float_.canFloat = true;	
		sandSlow_->Play();
	}

	return rot;
}

void PlayerBehavior::ProcessMoveSkill(float interval, float delta)
{
	if (!blaster_->IsPlay())
	{
		return;
	}
	// �r�[���̔����Ō�ɉ�����
	transform_->Pos().x += sinf(angles_.y);
	transform_->Pos().z += cosf(angles_.y);
}

float PlayerBehavior::ProcessDash(Controller& controller, const float& delta)
{
	auto speed = movedis_.walk;
	// �X�^�~�i�؂ꂵ�Ă�����ʂ���
	if (isStaminaLoss_)
	{
		return speed;
	}

	// ��𐧌�
	if (controller.Pressed(InputID::Dash))
	{
		// �_�b�V����
		isDodge_ = true;
		isDash_ = true;
		// �_�b�V�������n�_�ł̃G�t�F�N�g
		blaster_->Play();
		sand_->Play();
		sandDiff_->Stop();
		sandSlow_->Stop();
		// �T�E���h
		sound_.at(static_cast<int>(SOUNDNAME_SE::playerDash)).second = true;
		lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerAB,true);
	}
	else if (dodgeTimer_ > DodgeLimit)
	{
		// ����I���
		isDodge_ = false;
	}
	else if (controller.Released(InputID::Dash))
	{
		// �_�b�V���I���(������܂�)
		isDodge_ = false;
		isDash_ = false;
	}
	// �_�b�V������
	if (controller.Press(InputID::Dash) && (controller.GetLeftInput().SqMagnitude() > 0.0f))
	{
		speed = isDodge_ ? movedis_.dodge : movedis_.dash;									// ����̗L���ɂ��ړ��ʌv�Z
		(dodgeTimer_ >= 0.0f && isDodge_) ? dodgeTimer_ += delta : dodgeTimer_ = 0.0f;		// ������Ԓ��y�сA��𒆂Ȃ�^�C�}�[�o��
	}
	else if (controller.Released(InputID::Dash))
	{
		// �G�t�F�N�g
		blaster_->Stop();
		sand_->Stop();
		sandSlow_->Play();
		// �T�E���h
		lpSooundPross.SoundStop(SOUNDNAME_SE::playerAB);
	}
	return speed;
}

void PlayerBehavior::ProcessRotate(ObjectManager& objectManager, BaseScene& scene, Controller& controller, float rot)
{
	// ���f���̊p�x��ݒ�
	transform_->SetRotation({ angles_.x, angles_.y, angles_.z });
	if (!(controller.GetLeftInput().SqMagnitude() > 0.0f))
	{
		return;
	}

	// �ړ������ɃL�����N�^�[�̊p�x�����X�ɕς���
	float radUnitAnglesY = angles_.y;
	auto camAngle = camera_->GetRotation().ToEuler();

	float radMoveAnglesY = RadIn2PI(camAngle.y + rot);

	// ��]�ʂ����Ȃ����̉�]�������擾����i���v���F�P�A�����v���F�[�P�j
	float aroundDir = DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

	// true:�������l�ȓ��ɍ��킹�� false:��]�ʂ�������
	float diff = abs(radMoveAnglesY - radUnitAnglesY);
	auto rotRad = (isSkill_) ? SpeedRotRad / 10 : SpeedRotRad;
	angles_.y = (diff < 0.1f && !isSkill_) ? radMoveAnglesY : angles_.y + (rotRad * aroundDir);

	// angles_.y��0~360�x�ȓ��Ɏ��߂�
	angles_.y = RadIn2PI(angles_.y);
	collider_->SetMoveDir(-transform_->GetForward());
}


void PlayerBehavior::ProcessJump(Controller& controller)
{
	// �W�����v����
	auto hoverUD = (controller.Press(InputID::Jump) && !isStaminaLoss_) ? 1			// Up
		: !collider_->IsGround() ? -1												// Down
		: 0;
	if (hoverUD)
	{
		// ���x�X�V
		if ((transform_->Pos().y > 0.0f && hoverUD == -1) || (transform_->Pos().y < HoverPosYMax && hoverUD == 1))
		{
			jump_.vel += jump_.acc * hoverUD;
			collider_->SetGravityPow(jump_.vel);
		}
		collider_->SetGroundFlag(false);
		// ���W�X�V
		transform_->Pos().y = Clamp(transform_->Pos().y + transform_->GetUp().y * jump_.vel, 0.0f, HoverPosYMax);
		// �W�����v���͕��V���������Ȃ�
		float_.canFloat = false;
		sand_->Stop();
		sandDiff_->Stop();
		sandSlow_->Stop();
	}
	else
	{
		// �W�����v���Ă��Ȃ���		
		jump_.vel = 0.0f;
		if (float_.canFloat)
		{
			float_.canFloat = true;
		}
	}

	// �W�����v�����n�_�ł̃G�t�F�N�g
	if (controller.Pressed(InputID::Jump))
	{
		blaster_->Play();
		sand_->Stop();
		sandSlow_->Stop();
		lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerJump,false);
	}

	// �W�����v�̃G�t�F�N�g�̎�����
	if (controller.Released(InputID::Jump))
	{
		blaster_->Stop();
		sandDiff_->Play();
	}
}

void PlayerBehavior::ProcessFloat(void)
{
	auto floatUD = (float_.isUp) ? 1 : -1;									// Up : Down
	float_.vel += float_.acc * floatUD;										// ���V���x�v�Z
	auto disY = Clamp(transform_->GetUp().y * float_.vel, -0.14, 0.14);		// ���V�ӂꕝ�v�Z
	transform_->Pos().y += disY;											// ���W�X�V
	// ���V�����؂�ւ�
	float_.isUp = (disY >= 0.14) ? false									// ���~������
		: (disY <= -0.14) ? true											// �㏸������
		: float_.isUp;														// ����ێ�
}

void PlayerBehavior::ProcessAttack(Controller& controller, ObjectManager& objectManager)
{
	if (controller.Pressed(InputID::Attack) && !isSkill_ && !isAttack_)
	{
		// �R���{�U��
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime)
		{
			auto id = objectManager.CreateFromFactory(FactoryID::PlayerAttack, ownerId_, objectManager.GetComponent<Transform>(ownerId_)->GetPos());
			objectManager.Begin(id);
			auto attack = objectManager.GetComponent<PlayerAttackBehavior>(id);
			if (!(atkCnt_ % AtkAnimMax))
			{
				// �ŏ��̍U���͓����蔻��o���܂łɗ��߂̎��Ԃ��K�v
				attack->SetAttackStartTime(0.5f);
				// �U���Đ�����
				attack->SetAttackTime((animTotalTime - 30.0f) / 30.0f);
			}
			else
			{
				// �U���Đ�����
				attack->SetAttackTime(animTotalTime / 30.0f);
			}
			// �A�j���[�V�����C���f�b�N�X
			atkCnt_++;
			atkLimit_ = true;
			isAttack_ = true;
			// SE
			sound_.at(static_cast<int>(SOUNDNAME_SE::playerAttack)).second = true;
		}
	}
}

void PlayerBehavior::ResetCombo(void)
{
	atkCnt_ = 0;
	combo_ = 0;
	atkLimit_ = false;
}

void PlayerBehavior::ProcessStamina(Controller& controller)
{
	auto& value = gauge_.at(UiID::Stamina);
	auto& [min, max] = value.second;
	auto dashFlag = (controller.Press(InputID::Dash) && controller.GetLeftInput().SqMagnitude() > 0.0f && !isStaminaLoss_);
	// �_�b�V��
	value.first = (dashFlag && isDodge_) ? value.first - RiseValue				// ���
		: dashFlag ? value.first - RiseValue * 0.5								// �_�b�V��
		: value.first;															// ��
	// �W�����v
	auto jumpFlag = (controller.Press(InputID::Jump) && !isStaminaLoss_);
	value.first = (jumpFlag && jump_.vel >= 15.0f) ? value.first - RiseValue	// �W�����v������
		: (jumpFlag && jump_.vel < 15.0f) ? value.first - RiseValue * 0.5		// �W�����v������
		: value.first;															// ��

	// �_�b�V���A�W�����v�����Ă��Ȃ���΃X�^�~�i��
	if ((!controller.Press(InputID::Jump) && !(controller.Press(InputID::Dash) && (controller.GetLeftInput().SqMagnitude() > 0.0f))) || isStaminaLoss_)
	{
		value.first += RiseValue;
	}
	value.first = Clamp(value.first, min, max);
	// �X�^�~�i�؂�
	if (value.first <= min)
	{
		isStaminaLoss_ = true;
		isDodge_ = false;
		isDash_ = false;
		// �G�t�F�N�g
		blaster_->Stop();
		sand_->Stop();
		// �T�E���h
		lpSooundPross.SoundStop(SOUNDNAME_SE::playerAB);
	}
}

void PlayerBehavior::CoolTimer(float& delta)
{
	// �N�[���^�C��
	noDashTime_ += delta;
	if (noDashTime_ >= CoolTime)
	{
		isStaminaLoss_ = false;
		noDashTime_ = 0.0f;
	}
}

void PlayerBehavior::ProcessSkill(Controller& controller, ObjectManager& objectManager)
{
	if (controller.Pressed(InputID::Skil) && canSkill_)
	{
		// �K�E�X�L��
		auto id = objectManager.CreateFromFactory(FactoryID::PlayerSkill, ownerId_, transform_->GetPos());
		objectManager.Begin(id);

		auto skill = objectManager.GetComponent<PlayerSkillBehavior>(id);
		skill->SetAttackStartTime(2.4f);
		isSkill_ = true;
		// �u���X�^�[����
		blaster_->Play(2.4f);
		sandSlow_->Play(2.4f);
		sandDiff_->Stop();
		sand_->Stop();
		// �Q�[�W���Z�b�g
		gauge_.at(UiID::Skill).first = 0.0f;
		canSkill_ = false;
		sound_.at(static_cast<int>(SOUNDNAME_SE::playerSpetial)).second = true;
	}

}

void PlayerBehavior::RiseSkillValue(void)
{
	auto& value = gauge_.at(UiID::Skill);
	auto& [min, max] = value.second;
	// �X�L���Q�[�W�㏸
	value.first = Clamp(value.first + RiseValue * 30.0f, min, max);
	// �Q�[�WMAX
	if (value.first >= max)
	{
		canSkill_ = true;
	}
}

void PlayerBehavior::Animation(Controller& controller, ObjectManager& objectManager)
{
	// �R���{�U��
	auto index = AnimIndex::Idle;
	if (atkLimit_)
	{
		// �U���A�j���[�V����
		index = (atkCnt_ % AtkAnimMax == 1) ? AnimIndex::Attack1
			: (atkCnt_ % AtkAnimMax == 2) ? AnimIndex::Attack2
			: (atkCnt_ % AtkAnimMax == 0) ? AnimIndex::Attack3
			: index;
		// ���͎�t���ԓ��ɓ��͂�������΃R���{�I��
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime && animTotalTime != 0.0f)
		{
			isAttack_ = false;
			// �Đ����Ԓ��ߕ��̎���
			auto animPlayTimeOver = animation_->GetAnimState()->GetPlayTimeOver();
			if (animPlayTimeOver >= animTotalTime)
			{
				// �R���{���Z�b�g
				ResetCombo();
			}
		}
	}
	else
	{
		index = isSkill_ ? AnimIndex::Skill		// �K�E�Z
			: (controller.Press(InputID::Dash) && (controller.GetLeftInput().SqMagnitude() > 0.0f)) ? AnimIndex::ForwardMove		// �_�b�V���ړ�
			: (controller.GetLeftInput().SqMagnitude() > 0.0f) ? AnimIndex::MoveSlow		// �ړ�
			: (controller.IsNeutral()) ? AnimIndex::Idle		// ����
			: index;// �������Ȃ�
	}
	for (auto& tmp : anim_)
	{
		auto& [idx, time] = tmp;
		// index�ɑΉ������A�j���[�V����������
		if (index == idx)
		{
			animation_->SetNextAnimation(static_cast<int>(idx), time);
		}
	}
	
}

void PlayerBehavior::Sound(void)
{
	// ������
	for (auto& s : sound_)
	{
		if (s.second)
		{
			lpSooundPross.PlayBackSound(s.first,false);
			s.second = false;
		}
	}
}

void PlayerBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::EnemyAttack)
	{
		// ��ʗh��i�J�����V�F�C�N�j
		auto camera = objectManager.GetComponent<CameraBehavior>(objectManager.GetCameraID());
		if (camera.IsActive())
		{
			camera->StartShake(0.25f);
		}

		// �U���G�t�F�N�g
		auto attackTransform = objectManager.GetComponent<Transform>(col.GetOwnerID());
		auto effect = objectManager.CreateFromFactory(FactoryID::HitEffect, ownerId_, (transform_->GetPos()+ attackTransform->GetPos())/2.0f);
		objectManager.GetComponent<Transform>(effect)->Scale() = Vector3{ 0.3f,0.3f ,0.3f };
		objectManager.Begin(effect);

		// HP���炷
		auto power = objectManager.GetComponent<EnemyBehavior>(objectManager.GetEnemyID())->GetEnemyPower();
		auto& value = gauge_.at(UiID::Hp);
		auto& [min, max] = value.second;
		// HP����
		value.first = Clamp(value.first - power, min, max);
		// HP�S��
		if (value.first <= 0.0f)
		{
			// �v���C���[���ŏ���
			objectManager.GetComponent<ObjectInfo>(objectManager.GetPlayerID())->Destory();
			sound_.at(static_cast<int>(SOUNDNAME_SE::playerDestory)).second = true;
		}
		else
		{
			sound_.at(static_cast<int>(SOUNDNAME_SE::playerHit)).second = true;
		}
	}
}

void PlayerBehavior::CorrectionGroundPos(void)
{
	// �n�ʂɂ��Ȃ���΂ʂ���
	if (float_.groundPosY <= transform_->Pos().y)
	{
		return;
	}
	// �n�ʒ��n���ɒn�ʍ��W�X�V
	if (collider_->IsGround())
	{
		float_.groundPosY = transform_->Pos().y;
	}
	else
	{
		transform_->Pos().y = float_.groundPosY;
	}
}
