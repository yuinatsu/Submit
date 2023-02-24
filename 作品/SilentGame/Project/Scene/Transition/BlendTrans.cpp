#include <DxLib.h>
#include "BlendTrans.h"
#include "../../common/Debug.h"

BlendTrans::BlendTrans(double limitTime, uniqueScene beforScene, uniqueScene afterScene) :
	TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	addExt_ = 0.01f;
	DrawOwnScreen(0.0);
}

BlendTrans::~BlendTrans()
{
}

bool BlendTrans::UpdataTransition(double delta)
{
	count += delta;
	// limitTime�b�o�����玟�̃V�[���ֈȍ~
	// �g�����W�V�������I����Ă��違��ʉ�]��360�x�̎��_��true
	if (count > limitTime_ && (((float)((int)(afterScene_->GetAngle() * 10) % 63) / 10.0f) == 0))
	{
		return true;
	}
	// �I����Ă��Ȃ����false
	return false;
}

void BlendTrans::DrawOwnScreen(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(screenID_);
	//SetTransColor(255, 255, 255);
	GraphFilter(screenID_, DX_GRAPH_FILTER_GAUSS, 8, 30);
	auto alpha = static_cast<int>(255 * count / limitTime_);
	// �����ɂ���v�Z
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(Math::Vector2I(200,0), addExt_, -0.05);
	// �F������
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//afterScene_->Draw(addExt_, 0.1f);
	// �����ƗL���ɂȂ��Ă��܂��̂Ŗ����ɂ��閽��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
