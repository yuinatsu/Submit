#pragma once
#include "BaseScene.h"

// ���j���[��E�B���h�E���ɊJ���o�����邽�߂̃V�[��
class WindowScene :
	public BaseScene
{
public:

	WindowScene(SceneUptr owner, float actTime, ScreenID wndScreenID,ScreenID screenID,SceneID sceneID);

	/// <summary>
	/// �J��
	/// </summary>
	/// <param name=""></param>
	void Open(void);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name=""></param>
	void Close(void);
protected:
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �J�����o���I�����̍X�V����
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	virtual void UpdateOpend(float delta, Controller& controller) = 0;

	/// <summary>
	/// �E�B���h�E�����̕`��
	/// </summary>
	/// <param name=""></param>
	virtual void DrawWindow(void) = 0;

	/// <summary>
	/// ���鉉�o���I��������Ă΂��
	/// </summary>
	/// <param name=""></param>
	virtual void Closed(void);

	/// <summary>
	/// �w�i�ɓ����镔���̕`��(�f�t�H���g�ł͌Ăяo�����`��)
	/// </summary>
	/// <param name=""></param>
	virtual void DrawBackGround(void);

	/// <summary>
	/// �V�[����`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

	/// <summary>
	/// �J���Ƃ��̍X�V
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateOpen(float delta, Controller& controller);

	/// <summary>
	/// ���鎞�̍X�V
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateClose(float delta, Controller& controller);


	/// <summary>
	/// �J���Ƃ��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawOpen(void);

	/// <summary>
	/// ���鎞�̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary>
	/// �J������̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawOpend(void);

	// �X�V����
	void (WindowScene::* update_)(float, Controller&);


	// �`�揈��
	void (WindowScene::* draw_)(void);
	
	// �Ăяo����
	SceneUptr owner_;

	// �J�Ɏg���X�N���[��
	SharedRenderTargetHandle window_;

	// �J���o�ɂ����鎞��
	const float actTime_;

	// �o�ߎ���
	float step_;
};

