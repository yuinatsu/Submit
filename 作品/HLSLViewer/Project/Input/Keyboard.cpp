#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Keyboard.h"
#include "InputConfig.h"

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
}
