#include <array>
#include <DxLib.h>
#include "ColortoMono.h"

ColortoMono::ColortoMono(int postPS, Vector2 pos, Vector2 rate):PEBase(postPS, pos, rate)
{
	// �萔�o�b�t�@�̏�����
	timeBuff_ = CreateShaderConstantBuffer(sizeof(float) * 4);
	time_ = static_cast<float*>(GetBufferShaderConstantBuffer(timeBuff_));
	time = 0.0f;
}

ColortoMono::~ColortoMono()
{
}

void ColortoMono::Update(float delta)
{
	// ���X�Ƀ��m�N����������悤�ɍX�V����
	if (time < 1.0f)
	{
		time += delta /2.0f;
	}
	else
	{
		// 1.0�ȏ�ɂȂ�Ȃ��悤�ɒ�������
		time = 1.0f;
	}
}

int ColortoMono::Draw(int beforeScr, int afterScr, int depth, int skyScr, int redScr)
{
	SetDrawScreen(ownScr_);
	ClsDrawScreen();
	// �萔�o�b�t�@�Ɋi�[����
	time_[0] = time;
	// ���̃o�b�t�@��hlsl���ɓn��
	SetBuffer(timeBuff_);
	SetPostEffect(beforeScr, -1, -1, -1, postPS_, pos_, rate_);
	return ownScr_;
}
