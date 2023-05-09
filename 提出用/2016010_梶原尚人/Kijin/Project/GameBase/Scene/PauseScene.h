#pragma once
#include "WindowScene.h"


class PauseScene :
	public WindowScene
{
public:
	PauseScene(SceneUptr owner);

	/// <summary>
	/// �߂鏈��
	/// </summary>
	/// <param name=""></param>
	void BackGame(void);

	/// <summary>
	/// �I��(�_�C�A���O)
	/// </summary>
	/// <param name=""></param>
	void Dialog(void);

	/// <summary>
	/// �I�v�V����
	/// </summary>
	/// <param name=""></param>
	void Option(void);
private:
	const SceneID GetID(void) const
	{
		return SceneID::Pause;
	}

	/// <summary>
	/// �Q�[���V�[���֖߂鏈��
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeGameFunc(SceneUptr own);

	/// <summary>
	/// �_�C�A���O�V�[���𐶐�����֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeDialogFunc(SceneUptr own);

	/// <summary>
	/// �I�v�V�����V�[���𐶐�����֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeOptionFunc(SceneUptr own);


	void UpdateOpend(float delta, Controller& controller) final;

	void DrawWindow(void) final;

	void Closed(void) final;

	void Back(void) final;


	// �g�̃O���t�B�b�N�n���h��
	SharedGraphicHandle frame_;

	// �I��ID
	SceneID choiceNextID_;

	// �I���_�C�A���O�������͐ݒ��ʂ���̕��A�p
	Vector2 cursorPos_;
};

