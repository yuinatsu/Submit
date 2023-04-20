#pragma once
#include "WindowScene.h"
#include "../Common/Input/InputConfig/InputConfig.h"


// �ݒ��ʓ��̃V�[��
class OptionScene :
    public WindowScene
{
public:
	OptionScene(SceneUptr owner);

	/// <summary>
	/// InputSet�Ŏg��Input���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetInputID(const InputID id)
	{
		setID_ = id;
	}

	/// <summary>
	/// �w���InputID���w��̃R�[�h���g���悤�ɃZ�b�g����(InputSetScene����g�p�����)
	/// </summary>
	/// <param name="id"> id </param>
	/// <param name="code"> �g�p����R�[�h </param>
	void SetInputCode(InputID id, int code);

	/// <summary>
	/// ���݂̃C���v�b�g�R�[�h���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	InputCode& GetInputCode(void)&;

	/// <summary>
	/// �f�t�H���g�ɖ߂�
	/// </summary>
	/// <param name=""></param>
	void Default(void);

	/// <summary>
	/// �ݒ肵�����̂��g�p����(�ۑ�����)
	/// </summary>
	/// <param name=""></param>
	void Save(void);

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃��m�g�[���̎g�p�t���O���Z�b�g
	/// </summary>
	/// <param name="flag"></param>
	void SetPeMono(bool flag)
	{
		onPeMono_ = flag;
	}

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃��m�g�[�����g�p���邩?
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsPeMono(void) const
	{
		return onPeMono_;
	}

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃t�H�O�̃Z�b�g
	/// </summary>
	/// <param name="flag"></param>
	void SetPeFog(bool flag)
	{
		onPeFog_ = flag;
	}

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃t�H�O���g�p���邩�H
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsPeFog(void) const
	{
		return onPeFog_;
	}

private:
	const SceneID GetID(void) const final
	{
		return SceneID::Option;
	}

	void UpdateOpend(float delta, Controller& controller) final;

	void DrawWindow(void) final;

	void Closed(void) final;

	/// <summary>
	/// ���͂̐ݒ�V�[�������֐�
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeInputSetFunc(SceneUptr own);

	// �g
	SharedGraphicHandle frame_;

	// ui����w�肳�ꂽ���̂�InputSet�Ɏg��
	InputID setID_;

	// �ݒ肵������
	InputCode setResultcode_;

	// �|�X�g�G�t�F�N�g�̃��m�g�[���̐ݒ�
	bool onPeMono_;

	// �|�X�g�G�t�F�N�g�̃t�H�O�̐ݒ�
	bool onPeFog_;

};

