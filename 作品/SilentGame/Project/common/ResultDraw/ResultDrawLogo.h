#pragma once
#include "ResultDraw.h"
#include "../ShaderID.h"

template<class T>
class ConstantBuffer;

// ���U���g�V�[���Ō��ʂ�`�悷��(���S)
class ResultDrawLogo :
	public ResultDraw
{
public:
	ResultDrawLogo(const std::string& imgName,float startTime, const Math::Vector2& pos , float delta,bool IsClear);
	~ResultDrawLogo();
private:

	/// <summary> �`�悷�� </summary>
	/// <param name=""></param>
	void DrawOwn(void) final;

	// �摜�̃L�[��
	std::string imgName_;

	bool IsClear_;
	float delta_;

	// �萔
	float t;

	std::unique_ptr<ConstantBuffer<float>> cb_;

	// �s�N�Z���V�F�[�_��ID
	ShaderID psID_;
};

