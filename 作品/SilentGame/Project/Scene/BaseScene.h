#pragma once
#include<memory>
#include"../common/Math.h"
#include "../Input/Controller.h"

class BaseScene;
using uniqueScene = std::unique_ptr<BaseScene>;

/// <summary> �V�[���̎�� </summary>
enum class Scene
{
	Transtion,				// �V�[���J��
	Title,					// �^�C�g��
	Tutorial,				// �`���[�g���A��
	Select,					// �Z���N�g
	Game,					// �Q�[��
	Clear,					// �N���A
	Pause,					// �|�[�Y
	Option,					// �I�v�V����
	Result,					// ���U���g
	Load,					// ���[�h
	Max
};

class BaseScene
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	BaseScene();
	virtual ~BaseScene();

	/// <summary> ������ </summary>
	/// <returns> ���퓮��:true </returns>
	virtual bool Init(void) = 0;

	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="ownScene"> ���݂̃V�[�� </param>
	/// <returns> ���݂̃V�[�� </returns>
	virtual uniqueScene Update(float delta, uniqueScene ownScene) = 0;

	/// <summary> �`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Draw(float delta);

	virtual void Draw(float delta, Math::Vector2 pos, double extRate);

	/// <summary> �`�� </summary>
	/// <param name="extRate"> �g�嗦 </param>
	/// <param name="angle"> ��]�p�x </param>
	virtual void Draw(Math::Vector2I pos, double extRate, double angle);

	/// <summary> ���݂̃V�[���̕`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void DrawOwnScreen(float delta) = 0;

	/// <summary> ���݂̃V�[���̎擾 </summary>
	/// <returns> ���݂̃V�[�� </returns>
	virtual Scene GetSceneID(void) = 0;

	/// <summary> �V�[���J�ڃt���O�擾 </summary>
	/// <returns> �V�[���J��:true </returns>
	virtual bool GetFlag(void) = 0;

	virtual void SoundPlay(void);

	/// <summary> �p�x�擾 </summary>
	/// <returns> �p�x </returns>
	virtual double GetAngle(void) { return angle_; }

	/// <summary> ���[�h�I�����̏��� </summary>
	/// <param name=""></param>
	virtual void Loaded(void);
protected:
	/// <summary> �V�[���J�ڃt���O </summary>
	bool SceneFlag_;

	/// <summary> ��ʃT�C�Y </summary>
	Math::Vector2I screenSize_;

	/// <summary> �V�[���f�[�^�̊i�[ </summary>
	int screenID_;

	/// <summary> �R���g���[����� </summary>
	std::unique_ptr<Controller> controller_;

	/// <summary> �X�e�[�W�ԍ� </summary>
	int mapNum_;

	/// <summary> �g�嗦 </summary>
	double extRate_;

	/// <summary> �p�x </summary>
	double angle_;
	
	/// <summary> �T�E���h������ĉ��b�������� /// </summary>
	float loopTime_;
};

