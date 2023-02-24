#pragma once
#include "Collider.h"
#include "../Render/ModelRender.h"

class TerrainCollider :
    public Collider
{
public:
	TerrainCollider();
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;
	void End(ObjectManager& objectManager) final;

	static constexpr ComponentID id_{ ComponentID::MeshCollider };
	ComponentID GetID(void) const override
	{
		return id_;
	}

	/// <summary>
	/// 当たり判定に使うメッシュのハンドルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> ハンドル </returns>
	const int GetHandle(void) const;
	const int GetGroundHandle(void) const
	{
		return *ground_;
	}
	void Load(const std::filesystem::path& path);
	void LoadGround(const std::filesystem::path& path);
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

	// 当たり判定用モデルを表示するクラス(モデルハンドルを持っているので)
	ComponentHandle<ModelRender> render_;


	SharedModelHandle handel_;
	SharedModelHandle ground_;
};

