#pragma once
#include "TransitionScene.h"
class FadeSwap :
	public TransitionScene
{
public:
	FadeSwap(uniqueBaseScene beforScene, uniqueBaseScene afterScene);
	~FadeSwap();
private:
	void DrawOwnScreen() override;
	bool UpdateTransition(void) override;

	int count_;
};