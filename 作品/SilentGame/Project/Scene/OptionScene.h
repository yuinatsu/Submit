#pragma once
#include "BaseScene.h"

template<class T>
class MenuUICtrl;

class OptionBase;

enum class OSELECT
{
	Sound,// ����
	Key,// �L�[�R���t�B�O
	Exit,// �߂�
	Max,
};



class OptionScene :
	public BaseScene
{
public:
	OptionScene(uniqueScene gameScene);
	OptionScene(uniqueScene gameScene,uniqueScene beforScene,int mapNum, Scene scene);
	~OptionScene();
private:
	bool Init(void) override;

	uniqueScene Update(float delta, uniqueScene ownScene) override;
	void DrawOwnScreen(float delta) override;  // �������g��`��
 
	Scene GetSceneID(void) override { return Scene::Option; };
	bool GetFlag(void) override { return SceneFlag_; };


	/// <summary> �ݒ��Ԃ̍X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSetting(float delta, uniqueScene ownScene);

	/// <summary> �ʏ��Ԃ̎� </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSelect(float delta, uniqueScene ownScene);

	/// <summary> option�ֈڍs�������̉��o�p </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateOpen(float delta, uniqueScene ownScene);

	/// <summary> option���瑼�̃V�[���ֈڍs����Ƃ��̏��� </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateClose(float delta, uniqueScene ownScene);

	/// <summary> ������� </summary>
	void Relese();


	/// <summary> �ݒ��Ԃ̎��̕`�� </summary>
	/// <param name=""></param>
	void DrawSetting(void);

	/// <summary> �ʏ��Ԃ̕`�� </summary>
	/// <param name=""></param>
	void DrawSelect(void);

	/// <summary> option�ֈڍs�����Ƃ��̕`�� </summary>
	/// <param name=""></param>
	void DrawOpen(void);

	/// <summary> option���瑼�̃V�[���ֈڍs����Ƃ��̕`�� </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary> ����̐����̕\�� </summary>
	/// <param name=""></param>
	void DrawInput(void);

	/// <summary> ���j���[�̃E�B���h�E�̕\�� </summary>
	/// <param name=""></param>
	void DrawMenu(void);

	// uniqueScene beforScene_; // �߂������̃V�[��
	uniqueScene beforeScene_;// �`��p�̃V�[��

	// �X�V�����̃����o�֐��|�C���^
	uniqueScene(OptionScene::* update_)(float, uniqueScene);



	// �`�揈���̃����o�֐��|�C���^
	void (OptionScene::* draw_)(void);

	// �I�������ݒ荀�ڂ̃N���X
	std::unique_ptr< OptionBase> optionWnd_;

	// ���j���[UI���Ǘ�����N���X
	std::unique_ptr< MenuUICtrl<OSELECT>> menuUICtrl_;

	Scene scene_;

	// �o�ߎ���
	float time_;

	// �Q�[���V�[�����痈����?
	bool isBeforGameScene_;
 
	// option�̃E�B���h�E�̃X�N���[���n���h��
	int wndScreenH_;
};

