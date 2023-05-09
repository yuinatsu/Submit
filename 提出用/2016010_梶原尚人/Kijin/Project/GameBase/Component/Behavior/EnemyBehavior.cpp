#include "EnemyBehavior.h"
#include "../Transform/Transform.h"
#include "../../SceneManager.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/StageBehavior.h"
#include "PlayerBehavior.h"
#include "EnemyBulletBehavior.h"
#include "PlayerAttackBehavior.h"
#include "../../Common/SoundPross.h"
#include "../ComponentPool.h"
#include "EnemyAttackBehavior.h"


#include "../../Common/Debug.h"


// ��]���K�v�Ȕ͈�(�p�x)
constexpr float rotDeff{ Square(Deg2Rad(15.0f)) };

// �m�b�N�o�b�N���̏���
constexpr float v0{ 9.0f };

// �m�b�N�o�b�N�p�̏d�͉����x
constexpr float gravity{ 9.80f * 10.0f };

// �A�j���[�V�����C���f�b�N�X�p
enum class EnemyAnim
{
	Wait,			// �ҋ@���
	WalkHaveGun,	// �e�������ĕ���
	WalkHaveSword,	// �ߐڕ���������ĕ���
	GunToSword,		// �e����ߐڕ���֐؂�ւ���
	AttackSword,	// �ߐڕ���ōU������
	AttackGun,		// �e���\����
	GetHit,			// �U�����󂯂���
	SwordToGun		// �ߐڕ��킩��e�ɐ؂�ւ���
};

// �R���X�g���N�^
EnemyBehavior::EnemyBehavior()
{
	// �f�t�H���g�̃X�s�[�h�Ƃ���120�����Ƃ�
	speed_ = 120.0f;
	update_ = nullptr;
	stopDistance_ = 0.0f;
	searchDistance_ = 0.0f;
	// �f�t�H���g�̐U��������ԂƂ���1�����Ƃ�
	turnTime_ = 1.0f;
	Init();
}

// �X�V����
void EnemyBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stateTime_ += delta;

	// �d�͏���
	Gravity(delta);

	(this->*update_)(objectManager, delta);

}

// �J�E���^�ϐ��⎞�ԗp�ϐ��̏�����
void EnemyBehavior::Init(void)
{
	shotTime_ = 0.0f;
	burstCnt_ = 0;
	damageCnt_ = 0;
	isRot_ = false;
	stateTime_ = 0.0f;
	hitTime_ = 0.0f ;
	rotSing_ = 0.0f ;
	hitCombo_ = 0;
	jumpTime_ = 0.0f;
}


// �L�����̊J�n����
void EnemyBehavior::Begin(ObjectManager& objectManager)
{
	// �ϐ���������
	Init();

	// �g�����X�t�H�[�����擾
	transform_ = objectManager.GetComponent<Transform>(ownerId_);

	// �v���C���[�̃g�����X�t�H�[�����擾
	playerT_ = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	// �R���C�_�[���擾���q�b�g���̊֐����Z�b�g
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBehavior::OnHit, this,std::placeholders::_1, std::placeholders::_2));
	collider_->SetGroundFlag(true);

	// �X�V�p�֐����Z�b�g
	update_ = &EnemyBehavior::UpdateMove;

	if (isTutorial_)
	{
		// �`���[�g���A���̎��͒e�������Ȃ��悤�Z�b�g
		shot_ = &EnemyBehavior::NonShot;
	}
	else
	{
		shot_ = &EnemyBehavior::Shot;
	}
	
	// ���[�U�[�T�C�g�擾
	laserSight_ = objectManager.GetComponent<EnemyLaserSightBehavior>(laserSightID_);

	// �A�j���[�^�[�擾
	animetor_ = objectManager.GetComponent<Animator>(ownerId_);

	// �A�j���[�V�������Z�b�g
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::Wait), true);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::WalkHaveGun), true);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::GunToSword), false);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::AttackSword), false);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::AttackGun), true);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::GetHit), false);
	animetor_->AddAnimation(static_cast<int>(EnemyAnim::SwordToGun), false);
	animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::Wait));
}

// �I������
void EnemyBehavior::End(ObjectManager& objectManager)
{
	// �R���|�[�l���g���������ɂȂ������ɌĂ΂��
	auto stage = objectManager.GetComponent<StageBehavior>(stageID_);
	if (stage.IsActive())
	{
		// �X�e�[�W�������Ă��鎞�X�e�[�W�̓G�̃J�E���g�����炳����
		stage->SubEnemy(ownerId_);
	}
}

// �ǐՑO�̏�Ԃ̍X�V
void EnemyBehavior::UpdateSearch(ObjectManager& objectManager, float delta)
{
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(searchDistance_))
	{
		// ���̋����ɂȂ�����ړ��J�n����
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
		animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::WalkHaveGun), 0.5f);
	}
}

// �ˌ�����
void EnemyBehavior::Shot(ObjectManager& objectManager,float delta)
{
	shotTime_ += delta;
	if (shotTime_ >= 0.0f)
	{
		if (shotTime_ >= shotData_.burstInterval)
		{
			shotTime_ = 0.0f;
			burstCnt_++;
			auto shotPos = transform_->GetPos() + (transform_->GetRotation() * Vector3 { 10.0f, 190.0f, 120.0f });

			// �E������ɂ��炵�č쐬
			auto id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos);
			objectManager.Begin(id);
			// �}�Y���t���b�V���G�t�F�N�g
			auto effect = objectManager.CreateFromFactory(FactoryID::MuzzleFlashEffect, ownerId_, shotPos, { 0.0f,transform_->GetRotation().ToEuler().y + Deg2Rad(180.0f), 0.0f});
			objectManager.GetComponent<Transform>(effect)->Scale() = Vector3{ 0.5f,0.5f,0.5f };
			objectManager.Begin(effect);

			if (burstCnt_ >= shotData_.burstNum)
			{
				// �K��̐����˂�����0�ɂ���
				burstCnt_ = 0;
				shotTime_ = -shotData_.shotInterval;
			}
		}

		//// ���W�̎擾
		//auto bPos = VGet(shotPos.x, shotPos.y, shotPos.z);			// �e�̍��W

		//// �T�E���h�̍��W���擾
		//Set3DPositionSoundMem(bPos, static_cast<int>(SOUNDNAME_SE::enemyAttack));

		//// �T�E���h���͂��͈͂��w��
		//Set3DRadiusSoundMem(256, static_cast<int>(SOUNDNAME_SE::enemyAttack));
	}
}

// �ˌ����Ȃ��Ƃ��̏���
void EnemyBehavior::NonShot(ObjectManager& objectManager, float delta)
{
	// �Ȃɂ����Ȃ�
}

// �ǐՈړ���Ԃ̏���
void EnemyBehavior::UpdateMove(ObjectManager& objectManager, float delta)
{
	// ��]����
	Rotation(delta);

	// �ړ�
	transform_->Pos() += transform_->GetForward() * (delta * speed_);
	collider_->SetSpeed((delta * speed_));
	collider_->SetMoveDir(transform_->GetForward());

	if (!animetor_->GetAnimState()->IsBlend())
	{
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(stopDistance_))
		{
			// ���̋����ɂȂ������~��Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateStop;

			// ���[�U�[�T�C�g��\������
			laserSight_->On();

			// �e���\����A�j���[�V�������Z�b�g
			animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::AttackGun), 0.5f);
		}
		else if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(searchDistance_))
		{
			// ���̋����ȏ�ɂȂ�����ǐՑO�̏�Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateSearch;

			// �ҋ@��Ԃ̃A�j���[�V�������Z�b�g
			animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::Wait), 0.5f);
		}
	}
}

// ��~��Ԃ̏���
void EnemyBehavior::UpdateStop(ObjectManager& objectManager, float delta)
{
	// ��]
	Rotation(delta);

	if (!animetor_->GetAnimState()->IsBlend())
	{
		// �u�����h�I�����ɗ���

		// �ˌ�
		(this->*shot_)(objectManager, delta);
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(stopDistance_))
		{
			// ���ȏ�̋����ɂȂ�����ړ���Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateMove;
			animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::WalkHaveGun), 0.5f);
			laserSight_->Off();
		}
	}
}

// �U�����󂯂���̏��
void EnemyBehavior::UpdateHit(ObjectManager& objectManager, float delta)
{
	// ���ɉ�����
	transform_->Pos() += knockBackVec_ * (speed_ * 0.5f * delta);
	collider_->SetSpeed((delta * speed_));
	collider_->SetMoveDir(knockBackVec_);

	if (collider_->IsGround() && stateTime_ >= hitTime_ && !animetor_->GetAnimState()->IsBlend())
	{
		// �d�����ԕ��o������

		// ���Ԃ����Z�b�g
		stateTime_ = 0.0f;

		// �ߐڕ���֐؂�ւ����̏����ɂ���
		update_ = &EnemyBehavior::UpdateGunToSword;
		DebugLog("�d���I��");
		// 3��U�����󂯂������
		if (damageCnt_ >= HpMax)
		{
			DebugLog("�|���ꂽ");

			if (!isTutorial_)
			{
				// �ߐڍU������������폜����
				auto atkInfo = objectManager.GetComponent<ObjectInfo>(attackID_);
				if (atkInfo.IsActive() && *attackID_ != 0ull)
				{
					atkInfo->Destory();
				}
			}
			// ���M��j������
			objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();

			// �G�t�F�N�g���o��
			auto id = objectManager.CreateFromFactory(FactoryID::FireExEffect, ownerId_, transform_->GetPos());
			objectManager.Begin(id);

			// SE��炷
			lpSooundPross.PlayBackSound(SOUNDNAME_SE::enemyDestroy,false);
		}

		// ����؂�ւ��A�j���[�V�����J�n
		animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::GunToSword), 0.5f);

	}
}

void EnemyBehavior::UpdateGunToSword(ObjectManager& objectManager, float delta)
{
	if (animetor_->GetAnimState()->IsEnd())
	{
		// �ߐڍU���̃A�j���[�V�������Z�b�g
		animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::AttackSword), 0.5f);


		// �ߐڍU���̎��̂��̂ɕύX
		update_ = &EnemyBehavior::UpdateAttackSword;

		if (!isTutorial_)
		{
			// �ߐڍU���̃I�u�W�F�N�g�𐶐�
			attackID_ = objectManager.CreateFromFactory(FactoryID::EnemyAttack, ownerId_, transform_->GetPos());
			objectManager.GetComponent<EnemyAttackBehavior>(attackID_)->SetEnemyID(ownerId_);
			objectManager.Begin(attackID_);
		}
	}
}

void EnemyBehavior::UpdateAttackSword(ObjectManager& objectManager, float delta)
{
	if (animetor_->GetAnimState()->IsEnd())
	{
		// �A�j���[�V�����I����

		// �ߐڕ��킩��e�ɐ؂�ւ���A�j���[�V����������
		animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::SwordToGun), 0.5f);

		// �A�b�v�f�[�g�֐����ߐڕ��킩��e�ɐ؂�ւ��鎞�̂��̂ɕύX�ύX
		update_ = &EnemyBehavior::UpdateSwordToGun;

		if (!isTutorial_)
		{
			// �ߐڍU���̃I�u�W�F�N�g�̏���
			auto info = objectManager.GetComponent<ObjectInfo>(attackID_);
			if (info.IsActive() && *attackID_ != 0ull)
			{
				info->Destory();
				DebugLog("�U���I��", *attackID_);
			}

			// �����Ȃ��̂ɂ��Ă���
			attackID_ = {};
		}
	}
}

void EnemyBehavior::UpdateSwordToGun(ObjectManager& objectManager, float delta)
{
	if (!animetor_->GetAnimState()->IsBlend() && animetor_->GetAnimState()->IsEnd())
	{
		// �u�����h���A�j���[�V�������I�������Ƃ�

		// �e���\�����A�j���[�V�������Z�b�g
		animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::AttackGun), 0.5f);

		// ��~��Ԃ̂��̂ɕύX
		update_ = &EnemyBehavior::UpdateStop;
	}
}

// ��]����
void EnemyBehavior::Rotation(float delta)
{
	if (isRot_)
	{
		// ��]�����������̏���
		transform_->AddRotation({ 0.0f, Deg2Rad(60.0f) * rotSing_ * delta, 0.0f });
	}

	// �^�[�Q�b�g�܂ł̃x�N�g�������߂�
	auto targetVec{ playerT_->GetPos() - transform_->GetPos()};

	// �O���������擾
	auto forward{ transform_->GetForward() };

	// ��]���ׂ��p�x�����߂�
	auto rot = Square(std::atan2(targetVec.z, targetVec.x) - std::atan2(forward.z, forward.x));
	if (rot > rotDeff)
	{
		// ��]���ׂ���
		isRot_ = true;
		rotSing_ = Cross(Vector2{ forward.z, forward.x }, Vector2{ targetVec.z,targetVec.x }) > 0.0f ? 1.0f: -1.0f;
	}
	else
	{
		isRot_ = false;
	}
}

// �d�͏���
void EnemyBehavior::Gravity(float delta)
{
	if (!collider_->IsGround())
	{
		// �󒆂ɂ��鎞
		jumpTime_ += delta;
		float val = (v0 *jumpTime_) - (0.5f * gravity * Square(jumpTime_));
		transform_->Pos() += transform_->GetUp() * (val);
		collider_->SetGravityPow(val);
	}
	else
	{
		jumpTime_ = 0.0f;
	}
}


// �q�b�g���̏���
void EnemyBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::PlayerAttack)
	{
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		if (!player.IsActive())
		{
			// �v���C���[���A�N�e�B�u�o�Ȃ��Ƃ��������Ȃ�
			return;
		}

		if (hitCombo_ == player->GetAtkCnt())
		{
			// �����U���Ȃ̂ŏ������Ȃ�
			return;
		}
		if (damageCnt_ >= HpMax)
		{
			// �������łɎ��S���Ă���̂ŏ������Ȃ�
			return;
		}

		// �U�����󂯂��Ƃ��̏���
		hitCombo_ = player->GetAtkCnt();
		damageCnt_++;
		player->RiseSkillValue();
		player->AddCombo();

		// �����蔻��Ƃ̒��_�ɃG�t�F�N�g�𐶐�����
		auto hit = objectManager.GetComponent<Transform>(col.GetOwnerID());
		auto effect = objectManager.CreateFromFactory(FactoryID::HitEffect, ownerId_ ,(hit->GetPos() + transform_->GetPos()) / 2.0f);
		objectManager.GetComponent<Transform>(effect)->Scale() = Vector3{ 0.5f,0.5f,0.5f };
		objectManager.Begin(effect);

		// �m�b�N�o�b�N������
		KnockBack(objectManager);
	}
	else if (atr == ObjectAttribute::PlayerSkill)
	{
		if (damageCnt_ >= HpMax)
		{
			// �������łɎ��S���Ă���̂ŏ������Ȃ�
			return;
		}
		// �ꌂ���j
		damageCnt_ += HpMax;

		// �m�b�N�o�b�N������
		KnockBack(objectManager);
	}
}

void EnemyBehavior::KnockBack(ObjectManager& objectManager)
{
	// �U�����󂯂����̂��̂ɕύX
	update_ = &EnemyBehavior::UpdateHit;
	stateTime_ = 0.0f;

	// �W�����v������
	jumpTime_ = 0.0f;
	collider_->SetGroundFlag(false);
	transform_->Pos().y += 2.0f;

	// ���S���Ă�����q�b�g�X�g�b�v�𑽂߂ɂ���
	objectManager.StartHitStop(damageCnt_ >= HpMax ? 0.12f : 0.05f);

	// �v���C���[�̍��W����m�b�N�o�b�N���ׂ����������߂�
	auto playerTransform = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	if (playerTransform.IsActive())
	{
		knockBackVec_ = (transform_->GetPos() - playerTransform->GetPos());
		knockBackVec_.y = 0.0f;
		knockBackVec_.Normalize();
	}

	// �U�����󂯂����̃A�j���[�V�������Z�b�g
	animetor_->SetNextAnimation(static_cast<int>(EnemyAnim::GetHit), 0.125f);

	// �U�����󂯂�����SE��炷
	lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerAttackHit,false);
}
