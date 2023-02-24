#pragma once
#include <memory>
#include "../Common/Math.h"
#include"../Input/Controller.h"

class BaseScene;
using SceneUptr = std::unique_ptr<BaseScene>;

// �V�[��ID
enum class SceneID
{
	Viewer,			// �r���A�[
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// ������
	virtual bool Init(void) = 0;
	// �X�V
	virtual SceneUptr Update(SceneUptr ownScene, float delta) = 0;
	// �`��
	virtual void Draw(void);
	// ���݂̃V�[���̕`��
	virtual void DrawOwnScreen(float delta) = 0;
	// ���݂̃V�[���̎擾
	virtual SceneID GetSceneID(void) = 0;
protected:
	// �X�N���[���T�C�Y
	Vector2I screenSize_;
	// �X�N���[���f�[�^
	int screenID_;
	// �R���g���[�����
	std::unique_ptr<Controller> controller_;
};

