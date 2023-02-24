#include <DxLib.h>
#include "FadeInOut.h"

FadeInOut::FadeInOut(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene, const double& limitTime):
	TransitionScene(std::move(beforScene), std::move(afterScene)), limitTime_(limitTime)
{
}

FadeInOut::~FadeInOut()
{
}

bool FadeInOut::UpdateTransition(const double& delta)
{
	count_ += delta;
	return (count_ > limitTime_);
}

void FadeInOut::DrawOwnScreen(const double& delta)
{
	SetDrawScreen(screenID_);
	int alpha = static_cast<int>(255.0 * count_ / (limitTime_ / 2.0));
	if (limitTime_ / 2.0 > count_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
		beforScene_->Draw(delta);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);
		afterScene_->Draw(delta);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}
