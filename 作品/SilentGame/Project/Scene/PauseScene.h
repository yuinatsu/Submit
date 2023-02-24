#pragma once
#include <memory>
#include <functional>
#include <list>
#include "TitleScene.h"
#include "BaseScene.h"

enum class PSELECT
{
	Resume,// �ĊJ
	Retry,// ���g���C
	SelectScene,// �Z���N�g�V�[���ɑJ��
	Item,
	Setting,// �ݒ�
	Suspension,// �Q�[���I��
	Max,
};
template<class T>
class MenuUICtrl;

class ItemDescriptionWnd;

class PauseScene :
	public BaseScene
{
public:
	PauseScene(uniqueScene beforScene, int mapNum, bool isSmallMap,Scene scene);
	~PauseScene();
private:
	bool Init(void) override;
	uniqueScene Update(float delta, uniqueScene ownScene) override;
	
	/// <summary> �I����ʂ̃A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSetting(float delta, uniqueScene ownScene);

	/// <summary> �Q�[���I����I���� </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateEnd(float delta, uniqueScene ownScene);

	/// <summary> �A�C�e��������I���� </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateWnd(float delta, uniqueScene ownScene);

	/// <summary> �|�[�Y��ʂ��J�������̃A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateOpen(float delta, uniqueScene ownScene);

	/// <summary> �|�[�Y��ʂ���鎞�̃A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateClose(float delta, uniqueScene ownScene);


	/// <summary>  </summary>
	/// <param name="delta"></param>
	void UpdateConfirmation(float delta);
	
	
	void DrawOwnScreen(float delta) override;  // �������g��`��
	Scene GetSceneID(void) override { return Scene::Pause; };    // �����̓|�[�Y�V�[��
	bool GetFlag(void) override { return SceneFlag_; };
	
	/// <summary> �I����ʂ�Draw </summary>
	/// <param name="delta"></param>
	void DrawSetting(float delta);
	
	/// <summary> �I���I�����̕`�� </summary>
	/// <param name="delta"></param>
	void DrawEnd(float delta);

	/// <summary> �A�C�e�������I�����̕`�� </summary>
	/// <param name="delta"></param>
	void DrawWnd(float delta);

	/// <summary> �|�[�Y��ʂ��J�������̕`�� </summary>
	/// <param name="delta"></param>
	void DrawOpen(float delta);

	/// <summary> �|�[�Y��ʂ���鎞�̕`�� </summary>
	/// <param name="delta"></param>
	void DrawClose(float delta);
	
	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawKeyUD(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawKeyLR(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawItemKeyAll(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawItemKeyEnd(void);

	
	void Relese(void);
	

	//uniqueScene (PauseScene::* updata_)(float, uniqueScene);
	//void (PauseScene::* draw_)(float);

	// �A�b�v�f�[�g�p�����o�֐��|�C���^
	uniqueScene (PauseScene::* updateFunc_)(float, uniqueScene);

	// �`�揈���p�����o�֐��|�C���^
	void (PauseScene::* drawFunc_)(float);


	uniqueScene beforScene_;

	// ?
	int scene_;

	float time_;

	Scene oldScene_;

	// �����I�v�V�����V�[�����ǂ���
	bool isNextOption_;

	// ���j���[��UI���R���g���[������N���X
	std::unique_ptr<MenuUICtrl<PSELECT>> menuUICtrl_;

	// �A�C�e�������p�̃E�B���h�E���ۂ�����
	std::unique_ptr< ItemDescriptionWnd> itemWnd_;

	std::unique_ptr<MenuUICtrl<TST>> tstUICtrl_;

	// �|�[�Y��ʂ̃X�N���[���n���h��
	int pauseWndScreenH_;

	bool isSmallMap_;
};

