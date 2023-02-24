#pragma once
#include <DxLib.h>
#include "BaseScene.h"
#include <vector>
#include "../Common/SharedHandle.h"

struct LIGHT_MAT
{
	MATRIX view;
	MATRIX proj;
};

// ���C�g�J�����̈ʒu
constexpr Vector3 camPos{ 500.0f, 800.0f, -3000.0f };
// ���C�g�J�����̒����_
constexpr Vector3 camTar{ 0.0f, 0.0f, -1000.0f };
// ���C�g�J�����̐��ˉe�̕\���͈�
constexpr float offsetOrtho = 12000.0f;
// ���C�g�J�����̎�O�̋����Ɖ��̋���
constexpr float offsetNear = 0.001f;
constexpr float offsetFar = 13000.0f;

class GameScene :
    public BaseScene
{
public:
	GameScene();

	void SetResult(ResultAttribute result)
	{
		result_ = result;
	}
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
	/// �X�V����
	/// </summary>
	/// <param name="delta"></param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

	void SetupShadowMap(void);

	void SetupPostEffect(void);

	void SetPostEffect(bool flag,int x,int y,int img,int Postps);

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

	// �v���C���[�̍��W���擾
	Vector3 pos_;

	// �V���h�E�}�b�v
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightM_;
	int cbuffer_;

	// �|�X�g�G�t�F�N�g
	int PostPS_;

	// �|�X�g�G�t�F�N�g�p
	int PostTex_;
	// �g�p���钸�_�V�F�[�_
	std::vector<SharedShaderHandle> useShaders_;

	ResultAttribute result_;
};

