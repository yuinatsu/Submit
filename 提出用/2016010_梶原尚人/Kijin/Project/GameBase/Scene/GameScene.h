#pragma once
#include <DxLib.h>
#include "BaseScene.h"
#include <vector>
#include "../Common/SharedHandle.h"
#include "StageID.h"

struct DepthParam
{
	float start;
	float end;
	float scope;
};

struct WEIGHT_NUM
{
	float weight[8];
};

// �G�Ƃ̋����Ɗp�x���m��
struct MiniMapRadar
{
	float angle;
	float range;
	bool flag;
};

class GameScene :
    public BaseScene
{
public:
	GameScene(StageID stageID);
	~GameScene();

	// ���C�g�J�����̈ʒu
	static constexpr Vector3 camPos{ 500.0f, 800.0f, -3000.0f };
	// ���C�g�J�����̒����_
	static constexpr Vector3 camTar{ 0.0f, 0.0f, -1000.0f };
	// ���C�g�J�����̐��ˉe�̕\���͈�
	static constexpr float offsetOrtho = 12000.0f;
	// ���C�g�J�����̎�O�̋����Ɖ��̋���
	static constexpr float offsetNear = 0.001f;
	static constexpr float offsetFar = 13000.0f;
	// �K�E�V�A���u���̏d��
	static constexpr int NUM_WEIGHTS = 8;

	// �ڂ��������Ƃڂ�������̕�ԋ���
	static constexpr float dofInterpSize = 7000;
	// �ڂ��������͈̔͂̒��S�ʒu
	static constexpr float dofFocus = 500.0;
	// �ڂ��������͈̔�
	static constexpr float dofFocusSize = 17000;

	// �~�j�}�b�v�ɓG��\������͈�
	static constexpr float RADAR_RANGE = 90;

	void SetResult(ResultAttribute result)
	{
		result_ = result;
	}

	/// <summary>
	/// ��ʂ��L���v�`������
	/// </summary>
	/// <param name=""></param>
	void Capture(void);

	/// <summary>
	/// �J�n����|�[�Y��ʂ���̕��A��ɍs������
	/// </summary>
	/// <param name=""></param>
	void SetUp(void);

private:
	const SceneID GetID(void) const
	{
		return SceneID::Game;
	}

	/// <summary>
	/// ���U���g�V�[���ւ̐����֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeResultFunc(SceneUptr own);

	/// <summary>
	/// �|�[�Y�V�[�������֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakePauseFunc(SceneUptr own);

	/// <summary>
	/// �Z���N�g�V�[�������֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeSelectFunc(SceneUptr own);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta"></param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;
	/// <summary>
	/// �V���h�E�}�b�v�̍쐬
	/// </summary>
	/// <param name=""></param>
	void SetupShadowMap(void);
	/// <summary>
	/// ��ʊE�[�x�p�̐[�x�e�N�X�`��
	/// </summary>
	/// <param name=""></param>
	void SetUpDepth(void);
	/// <summary>
	/// �|�X�g�G�t�F�N�g�Ŏg���X�N���[���̍쐬
	/// </summary>
	/// <param name=""></param>
	void SetOffsetScreen(void);

	/// <summary>
	/// ���C���ȊO�̃X�N���[���̍쐬
	/// </summary>
	/// <param name=""></param>
	void SetSubScreen(void);

	/// <summary>
	/// ���[�h�������Ă��邩��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsLoaded(void) final;

	/// <summary>
	/// ���[�h�������̏���
	/// </summary>
	/// <param name=""></param>
	void Loaded(Controller& controller);

	void Back(void) final;

	// �V���h�E�}�b�v�p�̐[�x�e�N�X�`��
	// �V���h�E�}�b�v�p�V�F�[�_
	SharedShaderHandle shadowPs_;
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightMat;
	int shadowBuff_;

	// ��ʊE�[�x�p�̐[�x�e�N�X�`��
	// �[�x�e�N�X�`���p�V�F�[�_
	SharedShaderHandle depthPS_;
	int depth_;
	DepthParam* depthMat_;
	DepthParam depthMat;
	int depthbuffer_;

	float dofTotalSize_;

	// �I�t�X�N���[��
	int offScreen_;

	// �X�J�C�h�[���݂̂̃X�N���[��
	int skyScreen_;
	// �X�J�C�h�[���ƃX�e�[�W�݂̂̃X�N���[��
	int subScreen_;

	// �g�p���钸�_�V�F�[�_
	std::vector<SharedShaderHandle> useShaders_;

	ResultAttribute result_;

	// ���U���g�V�[���Ŏg���Q�[���I�����̃L���v�`���̕ۑ���
	SharedRenderTargetHandle resultCapture_;

	// ���݂̃X�e�[�WID
	StageID stageID_;

	friend class PauseScene;
};

