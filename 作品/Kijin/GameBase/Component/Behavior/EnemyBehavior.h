#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Transform/Transform.h"
#include "../Collider/CharactorCollider.h"

// �G�̓����p�̃N���X
class EnemyBehavior :
	public Behavior
{
public:
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

	enum class State
	{
		Search,
		Move,
		Stop,
		Hit
	};

private:
	static constexpr float Power = 20.0f;

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
	void Shot(float delta, ObjectManager& objectManager);
	
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
	/// ��]�̏���
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Rotation(float delta);

	/// <summary>
	/// �d�͏���(�m�b�N�o�b�N�p)
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Gravity(float delta);

	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �v���C���[�̃g�����X�t�H�[��
	ComponentHandle<Transform> playerT_;

	// ���g�̃R���C�_�[
	ComponentHandle<CharactorCollider> collider_;

	/// <summary>
	/// �q�b�g���̏���
	/// </summary>
	/// <param name="col"> ������������̃R���C�_�[ </param>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	// �ړ��X�s�[�h
	float speed_;

	// �W�����v�̃X�s�[�h
	float jumpTime;

	// ��~���鋗��
	float stopDistance_;

	// �T�m�J�n���鋗��
	float searchDistance_;

	// �ˌ��p
	float shotTime_;

	// ��e�J�E���g
	int damageCnt_;

	// �󂯂��U���̃R���{
	int hitCombo_;

	// ���݂̏�ԂɑJ�ڂ��Ă���̎���
	float stateTime_;

	// ��]�J�n���̊p�x
	float startRot_;

	// ��]��̊p�x
	float targetRot_;

	// ��]����܂ł̎���
	float rotTime_;

	// ��]���ĐU������I����܂ł̎���
	float turnTime_;

	// �U�����󂯂��ۂ̍d������
	float hitTime_;

	// �X�V����
	void (EnemyBehavior::* update_)(ObjectManager&, float);

	// �X�e�[�W�I�u�W�F�N�g��ID
	ObjectID stageID_;

	// �m�b�N�o�b�N����
	Vector3 knockBackVec_;
};

