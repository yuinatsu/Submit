#pragma once
#include "TransitionScene.h"
class CrossOver :
	public TransitionScene
{
public:
	CrossOver(uniqueBaseScene beforScene,uniqueBaseScene afterScene);		// CrossOver(�ύX�O�̃V�[��, �ύX��̃V�[��)
	~CrossOver();
private:
	bool UpdateTransition(double delta) override;							// Tansition�̍X�V UpdateTransition(deltaTime)
	void DrawOwnScreen(double delta) override;								// �e�V�[���̕`�� DrawOwnScreen(deltaTime)
};

