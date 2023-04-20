#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Transform/Transform.h"
#include "../Collider/CharactorCollider.h"
#include "EnemyLaserSightBehavior.h"
#include "../Animator/Animator.h"

// �G�̓����p�̃N���X
class EnemyBehavior :
	public Behavior
{
public:

	// �ˌ��p�f�[�^
	struct ShotData
	{
		float shotInterval;			// ���܂ł̃C���^�[�o��
		float burstInterval;		// �o�[�X�g�ˌ��̈ꔭ������̃C���^�[�o��
		int burstNum;				// �o�[�X�g�ˌ��̔��ː�
	};

	EnemyBehavior();

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::EnemyBehavior };

	/// <summary>
	/// �ړ��X�s�[�h�̃Z�b�g
	/// </summary>
	/// <param name="speed"> �X�s�[�h </param>
	void SetSpeed(const float speed)
	{
		speed_ = speed;
	}

	float GetEnemyPower(void){return Power;}

	/// <summary>
	/// �U�����󂯂��ۂ̍d�����Ԃ��Z�b�g
	/// </summary>
	/// <param name="hitTime"> �d������ </param>
	void SetHitTime(const float hitTime)
	{
		hitTime_ = hitTime;
	}

	/// <summary>
	/// �U������I���ɂ����鎞�Ԃ��Z�b�g
	/// </summary>
	/// <param name="turnTime"> �U������I���܂ł̎��� </param>
	void SetTurnTime(const float turnTime)
	{
		turnTime_ = turnTime;
	}

	/// <summary>
	/// ��~���鋗�����Z�b�g
	/// </summary>
	/// <param name="distance"> ��~����܂ł̋��� </param>
	void SetStopDistance(const float distance)
	{
		stopDistance_ = distance;
	}

	/// <summary>
	/// �X�e�[�W�̃I�u�W�F�N�g��ID
	/// </summary>
	/// <param name="id"> �X�e�[�W�I�u�W�F�N�g��ID </param>
	void SetStageID(const ObjectID& id)
	{
		stageID_ = id;
	}

	/// <summary>
	/// �G�̒ǐՊJ�n�܂ł̋������Z�b�g
	/// </summary>
	/// <param name="distance"> �ǐՊJ�n�܂ł̋��� </param>
	void SetSearchDistance(const float distance)
	{
		searchDistance_ = distance;
	}

	/// <summary>
	/// �`���[�g���A�����[�h�ɂ���
	/// </summary>
	/// <param name=""></param>
	void Tutorial(void)
	{
		isTutorial_ = true;
	}

	/// <summary>
	/// ���[�U�[�T�C�g�̃I�u�W�F�N�gID���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetLaserSightID(ObjectID& id)
	{
		laserSightID_ = id;
	}

	/// <summary>
	/// �ˌ��f�[�^���Z�b�g����
	/// </summary>
	/// <param name="data"></param>
	void SetShotData(const ShotData& data)
	{
		shotData_ = data;
	}

	/// <summary>
	/// ���[�U�[�T�C�g�̃I�u�W�F�N�gID���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID& GetlaserSightID(void)&
	{
		return laserSightID_;
	}

private:
	// �U����
	static constexpr float Power = 20.0f;
	// �ϋv�l
	static constexpr int HpMax = 3;

	/// <summary>
	/// ���Ԃ�J�E���^�[�p�ϐ�������������
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	void Begin(ObjectManager& objectManager) final;

	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// �ǐՑO�̒T�m��Ԃ̍X�V
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateSearch(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �e�̔��ˏ���
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Shot(ObjectManager& objectManager,float delta);
	
	/// <summary>
	/// ���˂��Ȃ��Ƃ�
	/// </summary>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	void NonShot(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �ǐՈړ����̏���
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateMove(ObjectManager& objectManager, float delta);

	/// <summary>
	/// ��~��Ԃ̏���
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateStop(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �U�����󂯂���Ԃ̏���
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateHit(ObjectManager& objectManager, float delta);

	/// <summary>
	/// ��������ւ��鎞�̏���
	/// </summary>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	void UpdateGunToSword(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �ߐڍU�����̏���
	/// </summary>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	void UpdateAttackSword(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �ߐڍU�����畐���؂�ւ��鎞
	/// </summary>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	void UpdateSwordToGun(ObjectManager& objectManager, float delta);

	/// <summary>
	/// ��]�̏���
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Rotation(float delta);

	/// <summary>
	/// �d�͏���(�m�b�N�o�b�N�p)
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Gravity(float delta);


	/// <summary>
	/// �q�b�g���̏���
	/// </summary>
	/// <param name="col"> ������������̃R���C�_�[ </param>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	/// <summary>
	/// �m�b�N�o�b�N����
	/// </summary>
	/// <param name="objectManager"></param>
	void KnockBack(ObjectManager& objectManager);


	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �v���C���[�̃g�����X�t�H�[��
	ComponentHandle<Transform> playerT_;

	// ���g�̃R���C�_�[
	ComponentHandle<CharactorCollider> collider_;

	// ���[�U�[
	ComponentHandle<EnemyLaserSightBehavior> laserSight_;

	// �A�j���[�^�[
	ComponentHandle<Animator> animetor_;

	// ���[�U�[�T�C�g��ID
	ObjectID laserSightID_;

	// �ړ��X�s�[�h
	float speed_;

	// �W�����v�̃X�s�[�h
	float jumpTime_;

	// ��~���鋗��
	float stopDistance_;

	// �T�m�J�n���鋗��
	float searchDistance_;

	// ��e�J�E���g
	int damageCnt_;

	// �󂯂��U���̃R���{
	int hitCombo_;

	// ���݂̏�ԂɑJ�ڂ��Ă���̎���
	float stateTime_;

	// ��]����(-1��1)
	float rotSing_;

	// ��]���s����?
	bool isRot_;

	// ��]���ĐU������I����܂ł̎���
	float turnTime_;

	// �U�����󂯂��ۂ̍d������
	float hitTime_;

	// �ˌ��Ɋւ���f�[�^
	ShotData shotData_;

	// �ˌ�����
	float shotTime_;

	// �o�[�X�g�ˌ���
	int burstCnt_;

	// �X�V����
	void (EnemyBehavior::* update_)(ObjectManager&, float);

	// �X�e�[�W�I�u�W�F�N�g��ID
	ObjectID stageID_;

	// �m�b�N�o�b�N����
	Vector3 knockBackVec_;

	// �`���[�g���A������?
	bool isTutorial_;

	// �ˌ�����
	void (EnemyBehavior::* shot_)(ObjectManager&, float);

	// �ߐڍU���̃I�u�W�F�N�gID
	ObjectID attackID_;
};

