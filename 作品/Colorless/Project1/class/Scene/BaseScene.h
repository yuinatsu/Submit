#pragma once
#include <array>
#include <memory>
#include "../common/Vector2.h"
#include "../common/ColorID.h"

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;

// �V�[���̎��
enum class Scene
{
	Transition,				// �V�[���ڍs
	Title,					// �^�C�g��
	Select,					// �X�e�[�W�Z���N�g
	Game,					// �Q�[�����s
	Clear,					// �N���A���
	Max
};

// �X�e�[�W�̎��
enum class Stage
{
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual bool Init(void) = 0;														// ������
	virtual uniqueBaseScene Update(double delta,uniqueBaseScene own) = 0;				// �X�V Update(deltaTime, �e�V�[��)
	virtual void Draw(double delta);													// �`�� Draw(deltaTime)
	virtual void DrawOwnScreen(double delta) = 0;										// �e�V�[���̕`�� Draw(deltaTime)
	virtual Scene GetSceneID(void) = 0;													// �e�V�[���̎擾
protected:
	int screenID_;																		// �X�N���[���i�[�p
	Vector2 screenSize_;																// �X�N���[���T�C�Y
};

