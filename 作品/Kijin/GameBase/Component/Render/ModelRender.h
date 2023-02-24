#pragma once
#include "Render.h"
#include "../../Common/SharedHandle.h"

// 3Dmodel��`�悷��N���X
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
	// ���f���̃n���h��
	SharedModelHandle handle_;
private:
	void Load(const std::filesystem::path& path) final;
	void LoadVertex(void);
	void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;

	// �g�p����V�F�[�_
	SharedShaderHandle ps_;
	SharedShaderHandle vs_;
	// �V���h�E�}�b�v�p�̕ϐ�
	int vs,ps;
	int tlbertType_;

	Vector3 boxPos1_;
	Vector3 boxPos2_;
};

