#pragma once
#include "BaseScene.h"
class SelectScene :
    public BaseScene
{
public:
	SelectScene();
private:
	const SceneID GetID(void) const
	{
		return SceneID::Select;
	}

	// �V�����Q�[���V�[���ւ̐����֐�
	SceneUptr MakeGameFunc(SceneUptr own);
	// �����̃Q�[���V�[���ւ̐����֐�
	//SceneUptr MakeReGameFunc(SceneUptr own);
	// �ݒ�V�[���ւ̐����֐�
	SceneUptr MakeOptionFunc(SceneUptr own);
	// �X�V����
	void Update(float delta, Controller& controller) final;
	// �V�[���̕`��
	void DrawScene(void) final;
};

