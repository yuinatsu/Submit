#include <Dxlib.h>
#include "Slide.h"
#include "../../Mng/SceneMng.h"

Slide::Slide(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene, const double& limitTime):
	TransitionScene(std::move(beforScene), std::move(afterScene)), limitTime_(limitTime)
{
	Init();
}

Slide::~Slide()
{
}

bool Slide::Init(void)
{
	const auto& scrSize = lpSceneMng.GetScreenSize();
	beforScreen_ = MakeScreen(scrSize.x, scrSize.y, true);
	SetDrawScreen(beforScreen_);
	beforScene_->Draw(0.0);
	afterScreen_ = MakeScreen(scrSize.x, scrSize.y, true);
	SetDrawScreen(afterScreen_);
	afterScene_->Draw(0.0);
	return true;
}

bool Slide::UpdateTransition(const double& delta)
{
	count_ += delta;
	return (count_ > limitTime_);
}

void Slide::DrawOwnScreen(const double& delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	const int& scrX = lpSceneMng.GetScreenSize().x;
	int moveX = static_cast<int>((scrX) * (count_ / limitTime_));
	DrawGraph(-moveX, 0, beforScreen_, true);
	DrawGraph(scrX - moveX, 0, afterScreen_, true);
}
