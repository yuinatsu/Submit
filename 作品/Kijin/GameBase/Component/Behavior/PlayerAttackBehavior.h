#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Collider/Collider.h"
#include "../Render/ModelRender.h"
#include "../Transform/Transform.h"

class PlayerAttackBehavior :
	public Behavior
{
public:
	PlayerAttackBehavior();
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	/// <summary>
	/// �U�����Ԃ��Z�b�g
	/// </summary>
	/// <param name="attackTime"></param>
	void SetAttackTime(float attackTime)
	{
		attackTime_ = attackTime;
	}

	/// <summary>
	/// �U�����肪�L���ɂȂ�܂ł̎���
	/// </summary>
	/// <param name="startTime"></param>
	void SetAttackStartTime(float startTime)
	{
		attackStartTime_ = startTime;
	}

	/// <summary>
	/// �R���{�̏����Z�b�g
	/// </summary>
	/// <param name="combo"></param>
	void SetComboNum(const int combo)
	{
		combo_ = combo;
	}

	/// <summary>
	/// �Đ��J�E���g���Z�b�g
	/// </summary>
	/// <param name="combo"></param>
	void SetPlayTime(const float time)
	{
		animTime_ = time;
	}

	/// <summary>
	/// �R���{���̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int GetComboNum(void) const
	{
		return combo_;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::PlayerAttackBehavior };
private:

	/// <summary>
	/// �I�u�W�F�N�g���L���ɂȂ������ɌĂ΂�鏈��
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	void Begin(ObjectManager& objectManager) final;

	/// <summary>
	/// �I�u�W�F�N�g�������ɂȂ������ɌĂ΂�鏈��
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[  </param>
	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// �q�b�g���̏���
	/// </summary>
	/// <param name="col"> ����R���C�_�[ </param>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[  </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	/// <summary>
	/// �j�����̏���
	/// </summary>
	/// <param name="objManager"> �I�u�W�F�N�g�}�l�[�W���[  </param>
	void Destory(ObjectManager& objManager) final;

	/// <summary>
	/// �U�����L���̎��̏���
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[  </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateAttack(ObjectManager& objectManager, float delta);

	/// <summary>
	/// �U�����L���ł͂Ȃ��Ƃ��̍X�V
	/// </summary>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[  </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateNonAttack(ObjectManager& objectManager, float delta);

	void (PlayerAttackBehavior::* update_)(ObjectManager& , float );

	// �U�����̎���
	float attackTime_;

	// �U���J�n�܂ł̎���
	float attackStartTime_;

	// ���g�̍U���̃R���{(�v���C���[����n�����)	
	int combo_;

	// �Đ��J�E���g
	float animTime_;

	// �R���C�_�[
	ComponentHandle<Collider> collider_;

	// �v���C���[�̃��f���`��N���X
	ComponentHandle<ModelRender> playerModel_;

	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �����蔻��̊�ɂ���frame��
	std::wstring frameName_;

	// �����蔻��p��frame�̃C���f�b�N�X
	int freameIdx_;


};

