#include "EnemyBehavior.h"
#include "../Transform/Transform.h"
#include "../../Application.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/StageBehavior.h"
#include "../../Common/Debug.h"
#include "PlayerBehavior.h"
#include "EnemyBulletBehavior.h"
#include "PlayerAttackBehavior.h"
#include "../../Common/SoundPross.h"

// ���ˈʒu�̏�����̂��炷��
constexpr float shotUpOffset{ 240.0f };

// ���ˈʒu�̑O�����̂��炷��
constexpr float shotForwardOffset{ 300.0f };

// ���ˈʒu�̉E�����̂��炷��
constexpr float shotRightOffset{ 60.0f };

// �e�𔭎˂���܂ł̎���
constexpr float shotTimeMax{ 2.5f };

// ��]���K�v�Ȕ͈�(�p�x)
constexpr float rotDeff{ Square(Deg2Rad(15.0f)) };

// �m�b�N�o�b�N���̏���
constexpr float v0{ 6.0f };

// �m�b�N�o�b�N�p�̏d�͉����x
constexpr float gravity{ 9.80f * 3.0f };

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
	damageCnt_ = 0;
	rotTime_ = 0.0f;
	stateTime_ = 0.0f;
	hitTime_ = 0.0f ;
	startRot_= 0.0f ;
	targetRot_ = 0.0f;
	hitCombo_ = 0;
}

// �L�����̊J�n����
void EnemyBehavior::Begin(ObjectManager& objectManager)
{
	// �ϐ���������
	Init();

	// �g�����X�t�H�[�����擾
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Pos().y += 5.0f;

	// �v���C���[�̃g�����X�t�H�[�����擾
	playerT_ = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	// �R���C�_�[���擾���q�b�g���̊֐����Z�b�g
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBehavior::OnHit, this,std::placeholders::_1, std::placeholders::_2));

	// �X�V�p�֐����Z�b�g
	update_ = &EnemyBehavior::UpdateMove;
}

// �I������
void EnemyBehavior::End(ObjectManager& objectManager)
{
	// �R���|�[�l���g���������ɂȂ������ɌĂ΂��
	auto stage = objectManager.GetComponent<StageBehavior>(stageID_);
	if (stage.IsActive())
	{
		// �X�e�[�W�������Ă��鎞�X�e�[�W�̓G�̃J�E���g�����炳����
		stage->SubEnemy();
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
	}
}

// �ˌ�����
void EnemyBehavior::Shot(float delta, ObjectManager& objectManager)
{
	shotTime_ += delta;
	if (shotTime_ >= shotTimeMax)
	{
		shotTime_ = 0.0f;

		// �O��E�̃x�N�g�����擾
		auto up = transform_->GetUp();
		auto forward = transform_->GetForward();
		auto right = transform_->GetRight();
		auto shotPos = transform_->GetPos();

		// ������ƑO�����ɂ��炷
		shotPos += up * shotUpOffset;
		shotPos += forward * shotForwardOffset;

		// �E������ɂ��炵�č쐬
		auto id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos + right * shotRightOffset);
		objectManager.Begin(id);

		// ��������ɂ��炵�č쐬
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos - right * shotRightOffset);
		objectManager.Begin(id);

		// �E�������ɂ��炵�č쐬
		shotPos += -up * 70.0f;
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_,  shotPos + right * 80.0f);
		objectManager.Begin(id);

		// ���������ɂ��炵�č쐬
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos - right * 80.0f);
		objectManager.Begin(id);

		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::enemyAttack, SoundProcess::GetVolume(), false);

		// �T�E���h�̍��W���擾
		//Set3DPositionSoundMem(shotPos), static_cast<int>(SoundProcess::SOUNDNAME_SE::enemyAttack));

		// �T�E���h���͂��͈͂��w��
		Set3DRadiusSoundMem(256.0f, static_cast<int>(SoundProcess::SOUNDNAME_SE::enemyAttack));
			
	}
}

// �ǐՈړ���Ԃ̏���
void EnemyBehavior::UpdateMove(ObjectManager& objectManager, float delta)
{
	// ��]����
	Rotation(delta);

	// �ړ�
	transform_->Pos() += transform_->GetForward() * (delta * speed_);
	
	// �ˌ�����
	Shot(delta, objectManager);

	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(stopDistance_))
	{
		// ���̋����ɂȂ������~��Ԃ�
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateStop;
	}
	else if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(searchDistance_))
	{
		// ���̋����ȏ�ɂȂ�����ǐՑO�̏�Ԃ�
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateSearch;
	}
}

// ��~��Ԃ̏���
void EnemyBehavior::UpdateStop(ObjectManager& objectManager, float delta)
{
	// ��]
	Rotation(delta);

	// �ˌ�
	Shot(delta, objectManager);
	
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(stopDistance_))
	{
		// ���ȏ�̋����ɂȂ�����ړ���Ԃ�
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
	}

}

// �U�����󂯂���̏��
void EnemyBehavior::UpdateHit(ObjectManager& objectManager, float delta)
{
	// ���ɉ�����
	transform_->Pos() += knockBackVec_ * (speed_) * delta;
	if (collider_->IsGround() && stateTime_ >= hitTime_)
	{
		// �d�����ԕ��o������
		stateTime_ = 0.0f;
		hitCombo_ = -1;
		update_ = &EnemyBehavior::UpdateMove;
		DebugLog("�d���I��");
		// 3��U�����󂯂������
		if (damageCnt_ >= 3)
		{
			DebugLog("�|���ꂽ");
			objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::enemyDestroy, SoundProcess::GetVolume(), false);
		}
	}
}

// ��]����
void EnemyBehavior::Rotation(float delta)
{
	
	rotTime_ -= delta;
	if (rotTime_ >= 0.0f)
	{
		// ��]�����������̏���
		auto rot = Learp(targetRot_, startRot_,rotTime_/ turnTime_);
		Vector3 lookVec{ std::cos(rot),0.0f, std::sin(rot) };
		transform_->LookAt(lookVec);
		return;
	}


	auto targetVec{ playerT_->GetPos() - transform_->GetPos()};
	auto forward{ transform_->GetForward() };
	auto rot = Square(std::atan2(targetVec.z, targetVec.x) - std::atan2(forward.z, forward.x));
	if (rot > rotDeff)
	{
		// ��]���ׂ���
		rotTime_ = turnTime_;
		startRot_ = std::atan2(forward.z, forward.x);
		targetRot_ = std::atan2(targetVec.z, targetVec.x);
	}
}

// �d�͏���
void EnemyBehavior::Gravity(float delta)
{
	if (!collider_->IsGround())
	{
		// �󒆂ɂ��鎞
		jumpTime += delta;
		float val = (v0 *jumpTime) - (0.5f * gravity * Square(jumpTime));
		transform_->Pos() += transform_->GetUp() * (val);
		collider_->SetGravityPow(val);
	}
	else
	{
		jumpTime = 0.0f;
	}
	//DebugDrawString("x=", transform_->GetPos().x, "y=", transform_->GetPos().y, "z=", transform_->GetPos().z, "veclocity=", jumpTime);
}

// �q�b�g���̏���
void EnemyBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::PlayerAttack)
	{
		auto attack = objectManager.GetComponent<PlayerAttackBehavior>(col.GetOwnerID());
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		if (hitCombo_ == attack->GetComboNum())
		{
			// �����U���Ȃ̂ŏ������Ȃ�
			//DebugLog("�����U��");
			return;
		}

		if (damageCnt_ >= 3)
		{
			// �������łɎ��S���Ă���̂ŏ������Ȃ�
			return;
		}
		DebugLog("�U���ʂ�");

		// �U�����󂯂��Ƃ��̏���
		//DebugLog("�U�����󂯂�!EnemyCombo=", hitCombo_, "AttackCombo=", attack->GetComboNum(), update_ == &EnemyBehavior::UpdateHit);
		damageCnt_++;
		player->RiseSkillValue();
		player->AddCombo();

		update_ = &EnemyBehavior::UpdateHit;
		stateTime_ = 0.0f;
		hitCombo_ = attack->GetComboNum();

		jumpTime = 0.0f;
		collider_->SetGroundFlag(false);
		transform_->Pos().y += 2.0f;
		objectManager.StartHitStop(0.05f);
		auto playerTransform = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
		if (playerTransform.IsActive())
		{
			knockBackVec_ = (transform_->GetPos() - playerTransform->GetPos());
			knockBackVec_.y = 0.0f;
			knockBackVec_.Normalize();
		}
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::playerAttackHit,SoundProcess::GetVolume(),false);
	}
}
