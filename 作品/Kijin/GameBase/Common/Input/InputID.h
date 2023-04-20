#pragma once
#include <type_traits>

// �L�[���
enum class InputID
{
	Jump,				// �W�����v
	Attack,				// �U��
	Dash,				// �_�b�V���g���K�[
	Skil,				// �X�L���̎g�p
	Max
};

static InputID begin(InputID) { return InputID::Jump; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
