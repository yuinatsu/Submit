#pragma once
#include <type_traits>

// �L�[���
enum class InputID
{
	Up,						// ��ړ�
	Down,					// ���ړ�
	Left,					// ���ړ�
	Right,					// �E�ړ�
	CameraUp,				// �J������ړ�
	CameraDown,				// �J�������ړ�
	CameraLeft,				// �J�������ړ�
	CameraRight,			// �J�����E�ړ�
	Dash,					// �_�b�V��
	Dimension,				// 2��������ύX
	Decision,				// ����
	btn1,					// ���j���[
	Max
};

static InputID begin(InputID) { return InputID::Up; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
