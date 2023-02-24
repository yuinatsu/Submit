#include <DxLib.h>
#include "FadeTrans.h"
FadeTrans::FadeTrans(double limitTime, uniqueScene beforScene, uniqueScene afterScene):
	TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	DrawOwnScreen(0.0);
}

FadeTrans::~FadeTrans()
{
}

bool FadeTrans::UpdataTransition(double delta)
{
	count += delta;
	// limitTime�b�o�����玟�̃V�[���ֈȍ~
	// �g�����W�V�������I����Ă����true
	if (count > limitTime_)
	{
		return true;
	}
	// �I����Ă��Ȃ����false
	return false;
}

void FadeTrans::DrawOwnScreen(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255 * count / limitTime_);
	// �����ɂ���v�Z
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	// �F������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	// �����ƗL���ɂȂ��Ă��܂��̂Ŗ����ɂ��閽��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}
