#pragma once
#include "WindowScene.h"

// �I�����Ɋm�F���邽�߂̃V�[��
class DialogScene :
	public WindowScene
{
public:
	DialogScene(SceneUptr owner);

	/// <summary>
	/// OK�̏���
	/// </summary>
	/// <param name=""></param>
	void Ok(void);

	/// <summary>
	/// Cancel�̏���
	/// </summary>
	/// <param name=""></param>
	void Cancel(void);
private:
	const SceneID GetID(void) const
	{
		return SceneID::Dialog;
	}

	/// <summary>
	/// �J������̍X�V����
	/// </summary>
	/// <param name="delta"> �f���^ </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void UpdateOpend(float delta, Controller& controller) final;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name=""></param>
	void DrawWindow(void) final;

	/// <summary>
	/// ���鉉�o���I��������̏���
	/// </summary>
	/// <param name=""></param>
	void Closed(void) final;

	// �g
	SharedGraphicHandle frame_;

	
};

