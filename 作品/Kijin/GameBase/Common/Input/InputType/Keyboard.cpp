#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Keyboard.h"
#include "../InputConfig/InputConfig.h"
#include "../../../SceneManager.h"
#include "../../Debug.h"

Keyboard::Keyboard()
{
	keyData_.fill(0);
	Init();
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Init(void)
{
	return true;
}

void Keyboard::Update(float delta)
{
	// �L�[�f�[�^���܂Ƃ߂Ď擾
	GetHitKeyStateAll(keyData_.data());

	// �}�E�X�̏�Ԃ��擾
	auto mouseState = GetMouseInput();

	for (auto& code : lpConfigMng.GetInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		if (code.second < 0)
		{
			// 0�ȉ��̎��̓}�E�X�Ɋւ�������Z�b�g����
			data_[static_cast<size_t>(code.first)].first = std::abs(code.second) & mouseState;
		}
		else
		{
			// �L�[�{�[�h�̏����Z�b�g����
			data_[static_cast<size_t>(code.first)].first = keyData_[code.second];
		}
	}

	// �L�[�{�[�h�̉�������Ԃɍ��킹�ĕ��������߂�
	leftInput_ = zeroVector2<float>;
	leftInput_.x += (keyData_[KEY_INPUT_W] ? -1.0f : 0.0f);
	leftInput_.x += (keyData_[KEY_INPUT_S] ? 1.0f : 0.0f);
	leftInput_.y += (keyData_[KEY_INPUT_D] ? -1.0f : 0.0f);
	leftInput_.y += (keyData_[KEY_INPUT_A] ? 1.0f : 0.0f);

	// ���K��
	leftInput_.Normalize();

	// �}�E�X�̍��W���擾
	int x = 0, y = 0;
	GetMousePoint(&x, &y);
	rightInput_ = cursorPos_ = Vector2{ static_cast<float>(x), static_cast<float>(y) };

	// ����{�^���̍X�V
	decisionData_.second = decisionData_.first;
	decisionData_.first = mouseState & MOUSE_INPUT_LEFT;

	// �L�����Z���{�^���̍X�V
	cancelData_.second = cancelData_.first;
	cancelData_.first = keyData_[KEY_INPUT_ESCAPE];
}

void Keyboard::SetCursorPos(const Vector2& pos)
{
	// �����̍��W���Z�b�g����
	SetMousePoint(static_cast<int>(pos.x), static_cast<int>(pos.y));
	cursorPos_ = pos;
}
