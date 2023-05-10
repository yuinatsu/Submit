#pragma once
#include "BaseScene.h"

class ObjectManager;


class TitleScene :
	public BaseScene
{
public:
	TitleScene();

	// ���C�g�J�����̒����_
	static constexpr Vector3 camTar{ 0.0f, 0.0f, -0 };
	// ���C�g�J�����̐��ˉe�̕\���͈�
	static constexpr float offsetOrtho = 1000.0f;
	// ���C�g�J�����̎�O�̋����Ɖ��̋���
	static constexpr float offsetNear = 0.001f;
	static constexpr float offsetFar = 2000.0f;

private:
	const SceneID GetID(void) const
	{
		return SceneID::Title;
	}
	SceneUptr MakeGameFunc(SceneUptr own);
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;
	bool IsLoaded(void) final;
	
	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateNon(float delta, Controller& controller);

	/// <summary>
	/// ���S�������Ƃ��̍X�V
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateLogoOff(float delta, Controller& controller);

	/// <summary>
	/// ���S�������Ƃ��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawLogoOff(void);

	/// <summary>
	/// �ʏ펞�̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawNon(void);

	/// <summary>
	/// �w�i�I�u�W�F�N�g�𐶐�����
	/// </summary>
	/// <param name=""></param>
	void CreateBackGround(void);

	/// <summary>
	/// ���[�h�������̏���
	/// </summary>
	/// <param name="controller"> �R���g���[���[ </param>
	void Loaded(Controller& controller);

	/// <summary>
	/// �V���h�E�}�b�v�̍쐬
	/// </summary>
	/// <param name=""></param>
	void SetupShadowMap(void);


	// �X�V����
	void (TitleScene::* update_)(float, Controller&);

	// �`�揈��
	void (TitleScene::* draw_)(void);

	// �o�ߎ���
	float stepTime_;

	// �V���h�E�}�b�v�p�̐[�x�e�N�X�`��
	// �V���h�E�}�b�v�p�V�F�[�_
	SharedShaderHandle shadowPs_;
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightMat;
	int shadowBuff_;

};

