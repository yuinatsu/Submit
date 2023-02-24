#pragma once
#include "TransitionScene.h"
class CrossOver :
	public TransitionScene
{
public:
	CrossOver(uniqueBaseScene beforScene, uniqueBaseScene afterScene);
	~CrossOver();
private:
	void DrawOwnScreen() override;
	bool UpdateTransition(void) override;

	int count_;
};

