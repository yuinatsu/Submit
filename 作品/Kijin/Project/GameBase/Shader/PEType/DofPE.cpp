#include <DxLib.h>
#include "DofPE.h"

#include "../../Common/Debug.h"

DofPE::DofPE(int postPS, Vector2 pos, Vector2 rate) :PEBase(postPS, pos, rate)
{
	DebugLog("DofPE�����J�n");
	VS_[0] = LoadVertexShader(L"Resource/resource/Shader/PostEffect/Xblur.vso");
	VS_[1] = LoadVertexShader(L"Resource/resource/Shader/PostEffect/Yblur.vso");
	ps_ = LoadPixelShader(L"Resource/resource/Shader/PostEffect/aveblur.pso");

	// �d�݂̏�����
	DebugLog("�d�ݏ�����");
	weibuff_ = CreateShaderConstantBuffer(sizeof(WEIGHT_NUM) * 4);
	weight = static_cast<WEIGHT_NUM*>(GetBufferShaderConstantBuffer(weibuff_));
	DebugLog("�d�݌v�Z");
	WeitghtsGaussian(NUM_WEIGHTS, 8.0f);
	weight[0] = weights_;

	// �X�N���[���T�C�Y�̏�����
	DebugLog("�X�N���[���T�C�Y������");
	scrbuff_ = CreateShaderConstantBuffer(sizeof(Vector2) * 4);
	screen = static_cast<Vector2*>(GetBufferShaderConstantBuffer(scrbuff_));
	screen_ = Vector2(0.0f, 0.0f);
	screen[0] = screen_;

	DebugLog("�X�N���[���T�C�Y����");
	int x, y;
	GetDrawScreenSize(&x, &y);
	vertBlur_ = MakeScreen(x / 2, y / 2, false);
	sideBlur_ = MakeScreen(x / 2, y, false);
	scr1_ = MakeScreen(x, y);
	// �����������Ɠ��A�Ɏ����������ȃ|�X�g�G�t�F�N�g���ł���
	scr2_ = MakeScreen(x, y);

	DebugLog("DofPE�����I��");
}

DofPE::~DofPE()
{
}

void DofPE::Update(float delta)
{
}

int DofPE::Draw(int beforeScr, int afterScr, int depth, int skyScr, int redScr)
{
	// �ア�u���[(���σu���[)
	SetDrawScreen(scr1_);
	ClsDrawScreen();
	SetPostEffect(beforeScr, -1, -1, -1, ps_, pos_, rate_);
	//SetBackgroundColor(0, 0, 0);

	// �����u���[
	// ���u���[��������
	DrawXblur(beforeScr);
	// �c�u���[��������
	DrawYblur();
	SetDrawScreen(scr2_);
	ClsDrawScreen();
	// �ł����u���[�����̃X�N���[���̃T�C�Y�ɖ߂�
	DefaultScreen(vertBlur_,afterScr, pos_, rate_);
	//SetBackgroundColor(128, 128, 128);

	SetDrawScreen(ownScr_);
	ClsDrawScreen();
	// ��ʊE�[�x�̂��ߏ��X�Ɏア�u���[�������Ă����āA�����Ă����Ɖ����Ƃ���͋����u���[��������
	SetPostEffect(beforeScr, scr1_,scr2_,depth,postPS_,pos_,rate_);
	return ownScr_;

}

void DofPE::DrawXblur(int beforeScr)
{
	SetDrawScreen(sideBlur_);
	ClsDrawScreen();
	SetXYBlurEffect(pos_, beforeScr, Vector2(2, 1), VS_[0]);
}

void DofPE::DrawYblur()
{
	SetDrawScreen(vertBlur_);
	ClsDrawScreen();
	SetXYBlurEffect(pos_, sideBlur_, Vector2(1, 2), VS_[0]);
}
