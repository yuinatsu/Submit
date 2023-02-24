#pragma once
#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
private:
	bool Init(void) override;												// ������
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// �X�V Update(deltaTime, �e�V�[��)
	void DrawOwnScreen(double delta);										// �e�V�[���̕`��
	Scene GetSceneID(void) override { return Scene::Title; };				// �e�V�[���̎擾
	Vector2F titlePos_;
	bool titleFlag_;
	double count_;
	bool logoFlag_;
	int titleBGM_;
	int titleSE_;
};

