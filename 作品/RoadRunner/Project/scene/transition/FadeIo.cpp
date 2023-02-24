#include "FadeIo.h"


FadeIo::FadeIo(uniqueBaseScene beforScene, uniqueBaseScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene))
{
	count_ = 0;
	DrawOwnScreen();
}


FadeIo::~FadeIo()
{
}


void FadeIo::DrawOwnScreen()
{
	SetDrawScreen(screenScrID_);
	ClearDrawScreen();
	if (count_ < 256)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - count_);
		beforScene_->Draw();
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, count_ - 255);
		afterScene_->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


bool FadeIo::UpdateTransition(void)
{
	DrawOwnScreen();
	count_ += 5;
	return (count_ > 510);				// ‘O‰‰ŽZ‚¶‚á‚È‚¢‚Æ”äŠr‰‰ŽZ‚ÌŒ‹‰Ê‚ª‚¸‚ê‚é
}
