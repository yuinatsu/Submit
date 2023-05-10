#pragma once
#include "Loading.h"

// ���[�h��ʂƃt�F�[�h���s���N���X
class FadeLoading :
    public Loading
{
public:
	FadeLoading(SceneUptr befor, SceneUptr after, float transTime);
	FadeLoading(SceneUptr after, float transTime);
private:

	/// <summary>
	/// �A�b�v�f�[�g����
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

	/// <summary>
	/// ���[�h�O�̃V�[�����t�F�[�h�A�E�g�����鎞�̍X�V
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateFadeOut(float delta, Controller& controller);

	/// <summary>
	/// ���[�h���̍X�V
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateLoading(float delta, Controller& controller);

	/// <summary>
	/// �J�ڌ�̃V�[�����t�F�[�h�C�������鎞�̍X�V
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateFadesIn(float delta, Controller& controller);

	/// <summary>
	/// �t�F�[�h�C�����̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawFadeIn(void);

	/// <summary>
	/// ���[�h���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawLoading(void);
	
	/// <summary>
	/// �t�F�[�h�A�E�g���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawFadeOut(void);

	// �X�V
	void (FadeLoading::* update_)(float, Controller&);

	// �`��
	void (FadeLoading::* draw_)(void);
};

