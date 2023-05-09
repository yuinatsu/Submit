#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Collider/Collider.h"
#include "../Render/ModelRender.h"
#include "../Transform/Transform.h"
#include "../../Factory/FactoryID.h"

class PlayerAttackBehavior :
	public Behavior
{
public:
	PlayerAttackBehavior();

	// �U�����Ԃ��Z�b�g 
	void SetAttackTime(float attackTime)
	{
		attackTime_ = attackTime;
	}

	// �U�����肪�L���ɂȂ�܂ł̎��� 
	void SetAttackStartTime(float startTime)
	{
		attackStartTime_ = startTime;
	}

	// �Đ��J�E���g���Z�b�g 
	void SetPlayTime(const float time)
	{
		animTime_ = time;
	}

	// �R���{���̎擾 
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
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Effect(ObjectManager& objectManager);
	void AddEffect(FactoryID id, ObjectManager& objectManager, const Vector3& offset);

	// �I�u�W�F�N�g���L���ɂȂ������ɌĂ΂�鏈�� 
	void Begin(ObjectManager& objectManager) final;

	// �I�u�W�F�N�g�������ɂȂ������ɌĂ΂�鏈�� 
	void End(ObjectManager& objectManager) final;

	// �q�b�g���̏��� 
	void OnHit(Collider& col, ObjectManager& objectManager);

	// �j�����̏��� 
	void Destory(ObjectManager& objManager) final;

	// �U�����L���̎��̏��� 
	void UpdateAttack(ObjectManager& objectManager, float delta);

	// �U�����L���ł͂Ȃ��Ƃ��̍X�V 
	void UpdateNonAttack(ObjectManager& objectManager, float delta);

	void (PlayerAttackBehavior::* update_)(ObjectManager&, float);

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
	std::wstring frameNameBlade_;
	std::wstring frameNameCenter_;

	// �����蔻��p��frame�̃C���f�b�N�X
	int frameIdx_;

	// �G�t�F�N�g
	std::vector<std::tuple<FactoryID, Vector3, bool>> effect_;

	// �f���^�^�C��
	float delta_;

	// �q�b�g���Ă���̎���
	float hitTime_;
	bool isAttackInterval_;
};

