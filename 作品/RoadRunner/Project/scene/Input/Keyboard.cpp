#include <DxLib.h>
#include "Keyboard.h"
#include "../../_debug/_DebugConOut.h"
#include "Joypad.h"


Keyboard::Keyboard()
{
	Init();
}


Keyboard::~Keyboard()
{
}


bool Keyboard::Update(Obj& obj)
{
	for (auto& data : data_)			// 入力情報がNOWかOLDかの情報取得
	{
		dataOld_[data.first] = data.second;
	}
	GetHitKeyStateAll(keyData_);
	for (auto tblData : keyTable_)
	{
		data_[tblData.first] = keyData_[tblData.second];				// 例）data["上"] = keyData_[keyTable_["上"]];
	}

	return true;
}


bool Keyboard::Init()
{
	keyTable_["上"] = KEY_INPUT_UP;
	keyTable_["下"] = KEY_INPUT_DOWN;
	keyTable_["左"] = KEY_INPUT_LEFT;
	keyTable_["右"] = KEY_INPUT_RIGHT;
	keyTable_["Ａ"] = KEY_INPUT_Z;
	keyTable_["エディット"] = KEY_INPUT_F6;


	for (auto tblData : keyTable_)			//
	{
		data_.emplace(tblData.first, 0);
	}

	// キーコンフィグ

	return true;
}
