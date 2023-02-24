#include "FadeSwap.h"

FadeSwap::FadeSwap(uniqueBaseScene beforScene, uniqueBaseScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene))
{
	count_ = 0;
	DrawOwnScreen();
}

FadeSwap::~FadeSwap()
{
}

void FadeSwap::DrawOwnScreen()
{
	SetDrawScreen(screenScrID_);
	ClearDrawScreen();
	if (count_ < 256)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 256 - count_);
		beforScene_->Draw();
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, count_ - 256);
		afterScene_->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool FadeSwap::UpdateTransition(void)
{
	DrawOwnScreen();
	count_ += 5;
	return (count_ > 255);				// ‘O‰‰ŽZ‚¶‚á‚È‚¢‚Æ”äŠr‰‰ŽZ‚ÌŒ‹‰Ê‚ª‚¸‚ê‚é
}
