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
	// キーデータをまとめて取得
	GetHitKeyStateAll(keyData_.data());
	// 入力情報格納
	for (auto& code : lpConfigMng.GetKeyInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = keyData_[code.second];
	}
}
