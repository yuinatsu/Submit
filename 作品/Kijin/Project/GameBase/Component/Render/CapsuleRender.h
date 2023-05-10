#pragma once
#include "Render.h"
#include "../../Common/Vector3.h"

// �J�v�Z���̕`��p�N���X
class CapsuleRender :
	public Render
{
public:
	CapsuleRender();
	~CapsuleRender();

	/// <summary>
	/// �J���[�̃Z�b�g
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const int color) { color_ = color; }

	/// <summary>
	/// �����̃Z�b�g
	/// </summary>
	/// <param name="height_"></param>
	void SetHeight(const float height) { height_ = height; }

	/// <summary>
	/// ���a�̃Z�b�g
	/// </summary>
	/// <param name="r"></param>
	void SetRadius(const float r) { radius_ = r; }

	static constexpr ComponentID id_{ ComponentID::CapsuleRender };
	ComponentID GetID(void) const override
	{
		return id_;
	}

private:
	void Draw(int shadowMap = -1, int buff = -1) final;
	void SetUpDepthTex(int ps = -1, int buff = -1) final;
	void Load(const std::filesystem::path& path) final;
	void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller) final;

	// ����
	float height_;

	// ���a
	float radius_;

	// �F
	int color_;

	// �g�b�v���W
	Vector3 top_;

	// �{�g�����W
	Vector3 bottom_;
};

