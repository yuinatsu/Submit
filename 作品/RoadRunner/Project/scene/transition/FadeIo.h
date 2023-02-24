#pragma once
#include "TransitionScene.h"
class FadeIo :
	public TransitionScene
{
public:
	FadeIo(uniqueBaseScene beforScene, uniqueBaseScene afterScene);
	~FadeIo();
private:
	void DrawOwnScreen() override;
	bool UpdateTransition(void) override;

	int count_;
};

