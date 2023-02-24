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
	// limitTime秒経ったら次のシーンへ以降
	// トランジションが終わっていればtrue
	if (count > limitTime_)
	{
		return true;
	}
	// 終わっていなければfalse
	return false;
}

void FadeTrans::DrawOwnScreen(float delta)
{
	// 自分自身のスクリーンに対してDraw
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255 * count / limitTime_);
	// 透明にする計算
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	// 色をつける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	// ずっと有効になってしまうので無効にする命令
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}
