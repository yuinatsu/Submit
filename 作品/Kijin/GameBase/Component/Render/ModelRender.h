#pragma once
#include "Render.h"
#include "../../Common/SharedHandle.h"

// 3Dmodelを描画するクラス
class ModelRender :
	public Render
{
public:
	ModelRender();
	virtual ~ModelRender();
	static constexpr ComponentID id_{ ComponentID::ModelRender };
	ComponentID GetID(void) const override
	{
		return id_;
	}
	void Draw(void) override;
	void Draw(int shadowMap, int buff) override;
	void SetUpShadowMap(void) override;

	static std::string Load(std::ifstream& file);

	SharedModelHandle& GetHandle(void) &
	{
		return handle_;
	}
protected:
	// モデルのハンドル
	SharedModelHandle handle_;
private:
	void Load(const std::filesystem::path& path) final;
	void LoadVertex(void);
	void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;

	// 使用するシェーダ
	SharedShaderHandle ps_;
	SharedShaderHandle vs_;
	// シャドウマップ用の変数
	int vs,ps;
	int tlbertType_;

	Vector3 boxPos1_;
	Vector3 boxPos2_;
};

