#include "CrossOver.h"


CrossOver::CrossOver(uniqueBaseScene beforScene, uniqueBaseScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene))
{
	//TRACE("CrossOver‚Ì¶¬\n");
	count_ = 0;
	DrawOwnScreen();
}


CrossOver::~CrossOver()
{
}


void CrossOver::DrawOwnScreen()
{
	SetDrawScreen(screenScrID_);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255 - count_);
	beforScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, count_);
	afterScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


bool CrossOver::UpdateTransition(void)
{
	DrawOwnScreen();

	//count_++;
	//if (count_ > 255)
	//{
	//	return true;
	//}
	//return false;
	// •Ê‰ð
	//-------
	return (++count_ > 255);				// ‘O‰‰ŽZ‚¶‚á‚È‚¢‚Æ”äŠr‰‰ŽZ‚ÌŒ‹‰Ê‚ª‚¸‚ê‚é				
}
