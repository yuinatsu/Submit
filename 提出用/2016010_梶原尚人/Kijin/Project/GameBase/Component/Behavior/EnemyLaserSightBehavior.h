#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Transform/Transform.h"
#include "../Render/LineRender.h"

// �G�̍U�����̎����̃N���X
class EnemyLaserSightBehavior :
    public Behavior
{
public:
	
	/// <summary>
	/// ��������G��ID���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetEnemyID(ObjectID& id)
	{
		enemyID_ = id;
	}

	/// <summary>
	/// ���[�U�[�T�C�g���I���ɂ���
	/// </summary>
	/// <param name=""></param>
	void On(void) const
	{
		line_->SetDrawFlag(true);
	}

	/// <summary>
	/// ���[�U�[�T�C�g���I�t�ɂ���
	/// </summary>
	/// <param name=""></param>
	void Off(void) const
	{
		line_->SetDrawFlag(false);
	}

	/// <summary>
	/// �I�t�Z�b�g���Z�b�g����
	/// </summary>
	/// <param name="offset"></param>
	void SetOffset(const Vector3& offset)
	{
		offset_ = offset;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}

	static constexpr ComponentID id_{ ComponentID::EnemyLaserSightBehavior };
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;


	// �������ׂ��G��ID
	ObjectID enemyID_;

	// �����悩��̃I�t�Z�b�g
	Vector3 offset_;

	// ������̓G
	ComponentHandle<Transform> enemy_;

	// ���g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// ����`�悷��N���X
	ComponentHandle<LineRender> line_;
};

