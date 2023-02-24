#pragma once
#include <functional>
#include <array>
#include "../../Input/InputID.h"
#include "OptionWindow.h"



enum class SettingType
{
	Dash,				// �_�b�V���g���K�[
	Crouch,				// ���Ⴊ�݃g���K�[
	ItemLeft,			// �A�C�e���I�����J�[�\��
	ItemRight,			// �A�C�e���I���E�J�[�\��
	Attack,				// �A�C�e���g�p�A�U���i�������ŗ��ߍU���j
	Init,		        // ������
	Exit,               // �߂�
	Max,
};

class KeyConfigOption :
	public OptionWindow<SettingType>
{
public:
	KeyConfigOption(Controller& controller);

private:

	bool Update(float delta) final;
	void Draw(void) final;

	bool UpdateKeyConfig(float delta);
	void DrawKeyConfig(void);
	void DrawPadConfig(void);
	void DrawKeyBoardConfig(void);

	bool UpdateSet(float delta);
	void DrawSet(void);

	// �L�[�{�[�h�p�̏���
	std::array<char, 256> key_;
	std::array<char, 256> oldkey_;
	// �Z�b�g����ID
	InputID inputID_;
	DINPUT_JOYSTATE state_;
	std::function<bool(float)> updata_;
	std::function <void(void)> draw_;
};

