#include <DxLib.h>
#include "Joypad.h"
#include "../../_debug/_DebugConOut.h"


Joypad::Joypad()
{
	Init();
}


Joypad::~Joypad()
{
}


bool Joypad::Update(Obj& obj)
{
	for (auto& data : data_)			// 入力情報がNOWかOLDかの情報取得
	{
		dataOld_[data.first] = data.second;
	}
	auto inputDat = GetJoypadInputState(DX_INPUT_PAD1);
	for (auto tblData : padTable_)			// 
	{
		data_[tblData.first] = inputDat & tblData.second;
	}

	data_["エディット"] = CheckHitKey(padTable_["エディット"]);

	return true;
}


bool Joypad::Init()
{
	padTable_["上"] = PAD_INPUT_UP;
	padTable_["下"] = PAD_INPUT_DOWN;
	padTable_["左"] = PAD_INPUT_LEFT;
	padTable_["右"] = PAD_INPUT_RIGHT;
	padTable_["Ａ"] = PAD_INPUT_1;

	keyTable_["エディット"] = KEY_INPUT_F6;

	for (auto tblData : padTable_)			// パッド操作
	{
		data_.emplace(tblData.first, 0);
	}
	for (auto tblData : keyTable_)			// キーボード操作
	{
		data_.emplace(tblData.first, 0);
	}

	return true;
}
