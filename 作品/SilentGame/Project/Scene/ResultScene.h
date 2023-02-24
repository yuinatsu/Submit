#pragma once
#include "BaseScene.h"
#include "../common/Math.h"

enum class RSELECT
{
	Retry,// ������x����
	Select,// �Z���N�g�V�[���ɑJ��
	Title,// �^�C�g���V�[��
	Max,
};

class ResultData;
class ResultDrawCtrl;

template<class T>
class MenuUICtrl;

class ResultScene :
	public BaseScene
{
public:
	ResultScene(ResultData& result,int mapNum, bool isSmallMap , float delta);
	~ResultScene();
private:
	bool Init(void) override;
	// �`��Ȃǂ̏�����
	void InitScreen(void);

	void Relese(void);

	uniqueScene Update(float delta, uniqueScene ownScene) final;

	// �I����ʂ�Update
	uniqueScene UpdateSelect(float delta, uniqueScene ownScene);

	void DrawOwnScreen(float delta) final;  // �������g��`��

	void DrawSelect(float delta);
	
	Scene GetSceneID(void) override { return Scene::Result; };    // �����̓��U���g�V�[��
	
	bool GetFlag(void) override { return SceneFlag_; };
	
	// ���ʂ̕\�����R���g���[������N���X
	std::unique_ptr< ResultDrawCtrl> resultDrawCtrl_;

	// ���j���[UI���R���g���[������N���X
	std::unique_ptr<MenuUICtrl<RSELECT>> menuUICtrl_;

	bool isSmallMap_;

	// ���u��
	float count_;
};

