#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "KeyInput.h"

KeyInput::KeyInput()
{
	Init();
}

KeyInput::~KeyInput()
{
}

bool KeyInput::Init(void)
{
	keyList_.try_emplace(InputID::Up, KEY_INPUT_UP);
	keyList_.try_emplace(InputID::Down, KEY_INPUT_DOWN);
	keyList_.try_emplace(InputID::Left, KEY_INPUT_LEFT);
	keyList_.try_emplace(InputID::Right, KEY_INPUT_RIGHT);
	keyList_.try_emplace(InputID::Btn1, KEY_INPUT_Z);						// 決定
	keyList_.try_emplace(InputID::Btn2, KEY_INPUT_X);						// キャンセル
	keyList_.try_emplace(InputID::Btn3, KEY_INPUT_C);						// 色変え欄表示
	keyList_.try_emplace(InputID::Btn4, KEY_INPUT_Q);						// ポーズ
	keyList_.try_emplace(InputID::Escape, KEY_INPUT_ESCAPE);
	keyList_.try_emplace(InputID::Jump, KEY_INPUT_UP);						// ジャンプ
	for (auto id : InputID())
	{
		cntData_[id].fill(false);
	}
	return true;
}

void KeyInput::Update(double delta)
{
	GetHitKeyStateAll(keyData_.data());
	// 更新
	for (auto&& id : InputID())
	{
		cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
		cntData_[id][static_cast<int>(Trg::Now)] = keyData_[keyList_[id]];
	}
}