#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Keyboard.h"
#include "InputConfig.h"
#include "../Common/Math.h"

Keyboard::Keyboard()
{
	keyData_.fill(0);
}

void Keyboard::Update(double delta)
{
	// �L�[�f�[�^���܂Ƃ߂Ď擾
	GetHitKeyStateAll(keyData_.data());
	// ���͏��i�[
	for (auto& code : lpConfigMng.GetKeyInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = keyData_[code.second];
	}

	// �L�[�{�[�h�̉�������Ԃɍ��킹�ĕ��������߂�
	leftInput_ = zeroVector2<float>;
	leftInput_.x += (keyData_[KEY_INPUT_W] ? -1.0f : 0.0f);
	leftInput_.x += (keyData_[KEY_INPUT_S] ? 1.0f : 0.0f);
	leftInput_.y += (keyData_[KEY_INPUT_D] ? -1.0f : 0.0f);
	leftInput_.y += (keyData_[KEY_INPUT_A] ? 1.0f : 0.0f);

	// ���K��
	leftInput_.Normalize();
}
