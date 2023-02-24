#pragma once
#include "../BaseScene.h"

// Transition�̎��
enum class Transition 
{
	CrossFade,
	FadeInOut,
	Max
};

class TransitionScene :
	public BaseScene
{
public:
	TransitionScene(uniqueBaseScene beforScene,uniqueBaseScene afterScene);			// TransitionScene(�ύX�O�̃V�[��, �ύX��̃V�[��)
	~TransitionScene();
private:
	bool Init(void) override;														// ������
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;				// �X�V Update(deltaTime, �e�V�[�����)
	virtual bool UpdateTransition(double delta) = 0;								// Tansition�̍X�V UpdateTransition(deltaTime)
protected:
	Scene GetSceneID(void) override final { return Scene::Transition; };
	uniqueBaseScene beforScene_;													// �ύX�O�̃V�[��
	uniqueBaseScene afterScene_;													// �ύX��̃V�[��
	double count_;																	// 1�t���[��������̒l
};

