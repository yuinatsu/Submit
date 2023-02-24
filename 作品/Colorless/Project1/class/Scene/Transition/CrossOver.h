#pragma once
#include "TransitionScene.h"
class CrossOver :
	public TransitionScene
{
public:
	CrossOver(uniqueBaseScene beforScene,uniqueBaseScene afterScene);		// CrossOver(変更前のシーン, 変更後のシーン)
	~CrossOver();
private:
	bool UpdateTransition(double delta) override;							// Tansitionの更新 UpdateTransition(deltaTime)
	void DrawOwnScreen(double delta) override;								// 各シーンの描画 DrawOwnScreen(deltaTime)
};

