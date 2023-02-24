#pragma once
#include "BaseScene.h"
class ClearScene :
	public BaseScene
{
public:
	ClearScene();
	~ClearScene();
private:
	bool Init(void) override;												// ������
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// �X�V Update(deltaTime, ���g�̃V�[��)
	void DrawOwnScreen(double delta);										// �e�V�[���̕`�� DrawOwnScene(deltaTime)
	Scene GetSceneID(void) override { return Scene::Clear; };				// �e�V�[���̎擾

	Vector2F cursorPos_;
	int cursorNum_;
	Vector2F shadowPos_;
	int blinkCnt_;
	int clearSE_;
};

