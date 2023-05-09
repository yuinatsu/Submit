#pragma once
#include "BaseScene.h"
#include "StageID.h"

class ResultScene :
    public BaseScene
{
public:
	ResultScene(StageID stageID,ResultAttribute atr, SharedRenderTargetHandle& capture);

	/// <summary>
	/// ���̃X�e�[�W�ֈڍs���鏈��������
	/// </summary>
	/// <param name=""></param>
	void NextStage(void);
private:
	const SceneID GetID(void) const
	{
		return SceneID::Result;
	}
	SceneUptr MakeSelectFunc(SceneUptr own);
	SceneUptr MakeRetryFunc(SceneUptr own);
	SceneUptr MakeDialogFunc(SceneUptr own);

	// ���j���[UI�̏��������܂Ƃ߂�
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;

	bool IsLoaded(void) final;

	void Back(void) final;

	// �Q�[���I�����̃L���v�`��
	SharedRenderTargetHandle capture_;

	// �w�i�̘g
	SharedGraphicHandle frame_;

	// �Q�[���V�[���̃X�e�[�WID
	StageID stageID_;

	// �_�C�A���O����̕��A���p
	Vector2 cursorPos_;
};
