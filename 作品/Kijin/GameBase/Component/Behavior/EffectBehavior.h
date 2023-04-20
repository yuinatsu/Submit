#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Render/EffectRender.h"
#include "../Transform/Transform.h"

enum class FactoryID;

// �����̎��
enum class EffectBehaviorType
{
	SyncPos,		// ���W������������
	SyncRot,		// ��]������������
	SyncAll,		// ���W�Ɖ�]������������
	Non				// �����������Ȃ���
};

// �ꕔ�G�t�F�N�g���Đ��I���Ɠ����ɏ��ł������萧�䂷��N���X
class EffectBehavior :
	public Behavior
{
public:
	EffectBehavior();

	/// <summary>
	/// �t�@�N�g���[ID�̃Z�b�g
	/// </summary>
	/// <param name="id"></param>
	void SetFactoryID(FactoryID id)
	{
		factoryID_ = id;
	}

	/// <summary>
	/// �����̎�ނ��Z�b�g����
	/// </summary>
	/// <param name="type"></param>
	void SetType(EffectBehaviorType type);

	/// <summary>
	/// ��������I�u�W�F�N�g��ID���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetSyncID(ObjectID& id)
	{
		syncID_ = id;
	}

	/// <summary>
	/// �I�t�Z�b�g���Z�b�g����
	/// </summary>
	/// <param name="offset"></param>
	void SetOffset(const Vector3 offset)
	{
		offset_ = offset;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}

	static constexpr ComponentID id_{ ComponentID::EffectBehavior };
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;
	void End(ObjectManager& objectManager) final;
	void Destory(ObjectManager& objManager) final;

	/// <summary>
	/// �����������Ȃ����̃A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateNonSync(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);
	
	/// <summary>
	/// ���W�����������鎞�̃A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSyncPos(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// ��]������������A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSyncRot(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// ��]�ƍ��W�����������鎞�̃A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSyncAll(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	// �A�b�v�f�[�g
	void(EffectBehavior::* update_)(BaseScene&, ObjectManager&, float, Controller&);

	// �G�t�F�N�g�̕`��N���X
	ComponentHandle<EffectRender> effectRender_;

	// ��������̃g�����X�t�H�[��
	ComponentHandle<Transform> syncTransform_;

	// �������g�̃g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �������̃t�@�N�g���[ID
	FactoryID factoryID_;

	// �I�t�Z�b�g
	Vector3 offset_;

	// ��������I�u�W�F�N�g��ID
	ObjectID syncID_;

	// ���
	EffectBehaviorType type_;
};

