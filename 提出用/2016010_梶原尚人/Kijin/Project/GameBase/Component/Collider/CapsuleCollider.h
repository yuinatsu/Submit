#pragma once
#include "Collider.h"

// �J�v�Z���^�̓����蔻��
class CapsuleCollider :
	public Collider
{
public:
	CapsuleCollider();

	/// <summary>
	/// �㑤�̍��W�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �㑤�̍��W </returns>
	const Vector3 GetTop(void) const;

	/// <summary>
	/// �����̍��W�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �����̍��W </returns>
	const Vector3 GetBottom(void) const;

	/// <summary>
	/// ���a�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���a </returns>
	const float GetRadius(void) const
	{
		return radius_;
	}

	/// <summary>
	/// ���a�̃Z�b�g
	/// </summary>
	/// <param name="radius"> ���a </param>
	void SetRadius(const float radius)
	{
		radius_ = radius;
	}

	/// <summary>
	/// �����̃Z�b�g
	/// </summary>
	/// <param name="height"> ���� </param>
	void SetHeight(const float height)
	{
		height_ = height;
	}

	static constexpr ComponentID id_{ ComponentID::CapsuleCollider };
	ComponentID GetID(void) const override
	{
		return id_;
	}
#ifdef _DEBUG
	void DrawDebug(void) final;
#endif
private:
	void Check(Collider& col, ObjectManager& objectManager) final;
	bool Check(CapsuleCollider& col, ObjectManager& objectManager) final;
	bool Check(SphereCollider& col, ObjectManager& objectManager) final;
	bool Check(MeshCollider& col, ObjectManager& objectManager) final;
	bool Check(CharactorCollider& col, ObjectManager& objectManager) final;
	bool Check(TerrainCollider& col, ObjectManager& objectManager) final;

	// ����
	float height_;

	// ���a
	float radius_;
};

