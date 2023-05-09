#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Collider/Collider.h"
#include "../Render/EffectRender.h"
#include "../Transform/Transform.h"
#include "../../Factory/FactoryID.h"
class PlayerSkillBehavior :
	public Behavior
{
public:
	PlayerSkillBehavior();

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

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::PlayerSkillBehavior };
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

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

	void (PlayerSkillBehavior::* update_)(ObjectManager&, float);

	// �U�����̎���
	float attackTime_;

	// �U���J�n�܂ł̎���
	float attackStartTime_;

	// �Đ��J�E���g
	float animTime_;

	// �R���C�_�[
	ComponentHandle<Collider> collider_;

	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	ObjectID effectID_;
};

