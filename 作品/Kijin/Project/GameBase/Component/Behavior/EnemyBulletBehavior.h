#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Transform/Transform.h"
#include "../Collider/SphereCollider.h"


class EnemyBulletBehavior :
	public Behavior
{
public:

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::EnemyBulletBehavior };
private:
	void Begin(ObjectManager& objectManager) final;

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	/// <summary>
	/// �q�b�g���̏���
	/// </summary>
	/// <param name="col"> ����̃R���C�_�[ </param>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	void Destory(ObjectManager& objManager) final;

	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �����蔻��̃N���X
	ComponentHandle<SphereCollider> collider_;

	// �J�n�ʒu
	Vector3 startPos_;
};

