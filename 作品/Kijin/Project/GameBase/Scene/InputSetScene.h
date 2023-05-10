#pragma once
#include "BaseScene.h"

// �L�[��PAD��ݒ�Ŏg���N���X(�ݒ肵�����{�^���������Ă��������݂����Ȃ��)
class InputSetScene :
    public BaseScene
{
public:
	InputSetScene(SceneUptr optionScene, InputID id);

private:
	const SceneID GetID(void) const final
	{
		return SceneID::InputSet;
	}

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
	/// �ҋ@�p�A�b�v�f�[�g
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateWait(float delta, Controller& controller);

	/// <summary>
	/// �L�[�{�[�h���Z�b�g����Ƃ��̃A�b�v�f�[�g
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateKeyboardSet(float delta, Controller& controller);

	/// <summary>
	/// �Q�[���p�b�h���Z�b�g����Ƃ��̃A�b�v�f�[�g
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdatePadSet(float delta, Controller& controller);

	/// <summary>
	/// �Z�b�g�I�����ĕ��鎞
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateClose(float delta, Controller& controller);

	/// <summary>
	/// �Z�b�g���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawSet(void);

	/// <summary>
	/// �Z�b�g�I�����ĕ���܂ł̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary>
	/// �Z�b�g�������l��o�^����
	/// </summary>
	/// <param name="code"></param>
	void SetCode(int code);

	// �X�V
	void (InputSetScene::* update_)(float, Controller& );

	// �`��
	void (InputSetScene::* draw_)(void);

	// �I�v�V�����V�[��
	SceneUptr optionScene_;

	// �Z�b�g����Ώۂ�ID
	InputID id_;

	// �o�ߎ���
	float step_;

	// ���͂𑣂����b�Z�[�W�̉摜
	SharedGraphicHandle mess_;

	// ���͂��ꂽ���Ƃ�m�点�郁�b�Z�[�W�̉摜
	SharedGraphicHandle mess2_;

	// ���͂ł��Ȃ����̂�I���������̃��b�Z�[�W�摜
	SharedGraphicHandle mess3_;

	// �I���ɐ����������H
	bool isSelectSuccess_;
};

