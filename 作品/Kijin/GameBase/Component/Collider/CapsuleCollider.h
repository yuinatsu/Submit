#pragma once
#include "Collider.h"

// ƒJƒvƒZƒ‹Œ^‚Ì“–‚½‚è”»’è
class CapsuleCollider :
	public Collider
{
public:
	CapsuleCollider();

	/// <summary>
	/// ã‘¤‚ÌÀ•W‚Ìæ“¾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ã‘¤‚ÌÀ•W </returns>
	const Vector3 GetTop(void) const;

	/// <summary>
	/// ‰º‘¤‚ÌÀ•W‚Ìæ“¾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ‰º‘¤‚ÌÀ•W </returns>
	const Vector3 GetBottom(void) const;

	/// <summary>
	/// ”¼Œa‚Ìæ“¾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ”¼Œa </returns>
	const float GetRadius(void) const
	{
		return radius_;
	}

	/// <summary>
	/// ”¼Œa‚ÌƒZƒbƒg
	/// </summary>
	/// <param name="radius"> ”¼Œa </param>
	void SetRadius(const float radius)
	{
		radius_ = radius;
	}

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

	// ‚‚³
	float height_;

	// ”¼Œa
	float radius_;
};

