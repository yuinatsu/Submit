#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver(uniqueBaseScene beforeScene, uniqueBaseScene afterScene) : TransitionScene(std::move(beforeScene), std::move(afterScene))
{
}

CrossOver::~CrossOver()
{
}

bool CrossOver::UpdateTransition(double delta)
{
	count_ += delta;
	if (count_ > 2.0)
	{
		return true;
	}
	return false;
}

void CrossOver::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	auto alpha = static_cast<int>(255.0 * count_ / 2.0);			// 255(α値のMAX) : 3.0(クロスオーバーの時間) = alpha(現在時間のα値) : count(処理開始からの経過時間)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}
