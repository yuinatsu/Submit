#pragma once
#include <type_traits>

// �L�[���
enum class InputID
{
	Up,					// ��ړ�
	Down,				// ���ړ�
	Left,				// ���ړ�
	Right,				// �E�ړ�
	Dash,				// �_�b�V���g���K�[
	Crouch,				// ���Ⴊ�݃g���K�[
	ItemLeft,			// �A�C�e���I�����J�[�\��
	ItemRight,			// �A�C�e���I���E�J�[�\��
	Attack,				// �A�C�e���g�p�A�U���i�������ŗ��ߍU���j
	btn1,				// �󂫂̃{�^���z�u
	Max
};

static InputID begin(InputID) { return InputID::Up; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
