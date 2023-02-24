#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene, const double& limitTime) :
	TransitionScene(std::move(beforScene), std::move(afterScene)), limitTime_(limitTime)
{
}

CrossOver::~CrossOver()
{
}

bool CrossOver::UpdateTransition(const double& delta)
{
	count_ += delta;
	return (count_ > limitTime_);
}

void CrossOver::DrawOwnScreen(const double& delta)
{
	SetDrawScreen(screenID_);
	int alpha = static_cast<int>(255.0 * count_ / limitTime_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}
