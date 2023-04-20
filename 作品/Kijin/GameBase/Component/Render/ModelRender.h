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
	void Draw(int shadowMap = -1, int buff = -1) override;
	void SetUpDepthTex(int ps = -1, int buff = -1) override;

	static std::string Load(std::ifstream& file);

	SharedModelHandle& GetHandle(void)&
	{
		return handle_;
	}

	/// <summary>
	/// ������Ԃ̊p�x���Z�b�g
	/// </summary>
	/// <param name="rot"></param>
	void SetDefaultRot(const Vector3& rot)
	{
		defaultRot_ = rot;
	}

	/// <summary>
	/// �J�����Ƃ̓����蔻��p�T�C�Y
	/// </summary>
	/// <param name="rt"></param>
	/// <param name="rb"></param>
	void SetBoundingSize(const Vector3& lt, const Vector3& rb)
	{
		bb_.isCheck_ = true;
		bb_.ltSize_ = lt;
		bb_.rbSize_ = rb;
	}


protected:
	// ���f���̃n���h��
	SharedModelHandle handle_;
	// �g�p����V�F�[�_
	SharedShaderHandle ps_;
	SharedShaderHandle vs_;
	// �V���h�E�}�b�v�p�V�F�[�_
	SharedShaderHandle shadowPs_;
	SharedShaderHandle shadowVs_;

private:

	// �J�����Ƃ̔���p
	struct CameraBB
	{
		CameraBB();
		bool IsHit(const Vector3& pos, const Quaternion& rot) const&;
		Vector3 ltSize_;
		Vector3 rbSize_;
		bool isCheck_;

	};

	void Load(const std::filesystem::path& path) final;
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;
	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// �J�������猩���邩?
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsCameraView(void);

	Vector3 defaultRot_;

	// �J�����̔���
	CameraBB bb_;

};

