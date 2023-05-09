#pragma once
#include "Render.h"

// �O�������ɐ���`�悷��N���X
class LineRender :
    public Render
{
public:

	/// <summary>
	/// ���̕`��F���Z�b�g����
	/// </summary>
	/// <param name="color"></param>
	void SetColor(int color)
	{
		color_ = color;
	}

	/// <summary>
	/// ���̒������Z�b�g����
	/// </summary>
	/// <param name="length"> ���� </param>
	void SetLength(float length)
	{
		length_ = length;
	}

	static constexpr ComponentID id_{ ComponentID::LineRender };
	ComponentID GetID(void) const override
	{
		return id_;
	}
private:

	void Load(const std::filesystem::path& path) final;
	void Draw(int shadowMap = -1, int buff = -1) final;
	void SetUpDepthTex(int ps = -1, int buff = -1)final;

	// �`�悷��F
	int color_;

	// ���̒���
	float length_;
};

