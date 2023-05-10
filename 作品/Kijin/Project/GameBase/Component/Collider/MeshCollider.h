#pragma once
#include "Collider.h"
#include "../ComponentHandle.h"
#include "../Render/ModelRender.h"

class MeshCollider :
	public Collider
{
public:
	MeshCollider();
	~MeshCollider();
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;
	void End(ObjectManager& objectManager) final;

	static constexpr ComponentID id_{ ComponentID::MeshCollider };
	ComponentID GetID(void) const override
	{
		return id_;
	}
	void Check(Collider& col, ObjectManager& objectManager) final;
	bool Check(CapsuleCollider& col, ObjectManager& objectManager) final;
	bool Check(SphereCollider& col, ObjectManager& objectManager) final;
	bool Check(MeshCollider& col, ObjectManager& objectManager) final;
	bool Check(CharactorCollider& col, ObjectManager& objectManager) final;
	bool Check(TerrainCollider& col, ObjectManager& objectManager) final;

	/// <summary>
	/// �����蔻��Ŏg�p���郁�b�V���̃n���h��
	/// </summary>
	/// <param name=""></param>
	/// <returns> �n���h�� </returns>
	const int GetHandle(void) const;


#ifdef _DEBUG
	void DrawDebug(void) final;
#endif
private:
	

	// �����蔻��p���f����\������N���X(���f���n���h���������Ă���̂�)
	ComponentHandle<ModelRender> render_;
};

