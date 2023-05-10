#pragma once
#include "Collider.h"
class SphereCollider :
    public Collider
{
public:
	SphereCollider();

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
	/// <param name="radius"> �Z�b�g���锼�a </param>
	void SetRadius(const float radius)
	{
		radius_ = radius;
	}

	/// <summary>
	/// ���W���擾(offset�l���ς݂�)
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���W </returns>
	const Vector3 GetPos(void) const;

	static constexpr ComponentID id_{ ComponentID::SphereCollider };
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


	// ���a
	float radius_;
};

