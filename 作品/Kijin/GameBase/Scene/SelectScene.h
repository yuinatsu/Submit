#pragma once
#include "WindowScene.h"
#include "StageID.h"

// �Z���N�g�V�[��
class SelectScene :
    public WindowScene
{
public:
	SelectScene();
	// ���C�g�J�����̒����_
	static constexpr Vector3 camTar{ 0.0f, 0.0f, -0 };
	// ���C�g�J�����̐��ˉe�̕\���͈�
	static constexpr float offsetOrtho = 1000.0f;
	// ���C�g�J�����̎�O�̋����Ɖ��̋���
	static constexpr float offsetNear = 0.001f;
	static constexpr float offsetFar = 2000.0f;

	SelectScene(std::unique_ptr<ObjectManager>&& objectManager);

	/// <summary>
	/// �I����(�_�C�A���O)
	/// </summary>
	/// <param name=""></param>
	void Dialog(void);

	/// <summary>
	/// �I�v�V�����V�[����
	/// </summary>
	/// <param name=""></param>
	void Option(void);

	/// <summary>
	/// �Q�[���V�[����
	/// </summary>
	/// <param name=""></param>
	void Play(void);

	/// <summary>
	/// �`���[�g���A����
	/// </summary>
	/// <param name=""></param>
	void Tutorial(void);
private:
	const SceneID GetID(void) const
	{
		return SceneID::Select;
	}

	// �V�����Q�[���V�[���ւ̐����֐�
	SceneUptr MakeGameFunc(SceneUptr own);

	// �ݒ�V�[���ւ̐����֐�
	SceneUptr MakeOptionFunc(SceneUptr own);

	// �_�C�A���O�����֐�
	SceneUptr MakeDialogFunc(SceneUptr own);

#ifdef _DEBUG
	SceneUptr MakeTitleFunc(SceneUptr own);
#endif

	void UpdateOpend(float delta, Controller& controller) final;

	void DrawWindow(void) final;

	void Closed(void) final;

	bool IsLoaded(void) final;

	void Init(void);

	void DrawBackGround(void) final;

	void Back(void) final;

	void Loaded(Controller& controller);
	
	/// <summary>
	/// �V���h�E�}�b�v�̍쐬
	/// </summary>
	/// <param name=""></param>
	void SetupShadowMap(void);

	// �I��ID
	SceneID choiceNextID_;

	// �g
	SharedGraphicHandle frame_;

	// �V���h�E�}�b�v�p�̐[�x�e�N�X�`��
	// �V���h�E�}�b�v�p�V�F�[�_
	SharedShaderHandle shadowPs_;
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightMat;
	int shadowBuff_;

	// �I�������X�e�[�W
	StageID staegID_;

	// �ݒ��ʂ��畜�A�������p�̃J�[�\���̈ʒu
	Vector2 cursorPos_;
};

