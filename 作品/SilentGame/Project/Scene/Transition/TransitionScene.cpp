#include <DxLib.h>
#include "TransitionScene.h"
#include "../../Object/ObjManager.h"

TransitionScene::TransitionScene(uniqueScene beforScene, uniqueScene afterScene)
{
	// ��̃V�[���̕ۑ�
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	// �J�E���g���Z�b�g
	count = 0;
	SceneFlag_ = false;
	// �`��
	Draw();
}

TransitionScene::~TransitionScene()
{
}

bool TransitionScene::Init(void)
{
	return false;
}

uniqueScene TransitionScene::Update(float delta, uniqueScene ownScene)
{
	// �g�����W�V���������Ŏ��̃V�[���ֈȍ~
	if (UpdataTransition(delta))
	{
		return std::move(afterScene_);
	}
	// �������Ȃ�g�����W�V�����`��
	DrawOwnScreen(delta);
	return ownScene;
}

void TransitionScene::Draw(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw(0);
}
