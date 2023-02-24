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
	for (auto& data : data_)			// ���͏��NOW��OLD���̏��擾
	{
		dataOld_[data.first] = data.second;
	}
	auto inputDat = GetJoypadInputState(DX_INPUT_PAD1);
	for (auto tblData : padTable_)			// 
	{
		data_[tblData.first] = inputDat & tblData.second;
	}

	data_["�G�f�B�b�g"] = CheckHitKey(padTable_["�G�f�B�b�g"]);

	return true;
}


bool Joypad::Init()
{
	padTable_["��"] = PAD_INPUT_UP;
	padTable_["��"] = PAD_INPUT_DOWN;
	padTable_["��"] = PAD_INPUT_LEFT;
	padTable_["�E"] = PAD_INPUT_RIGHT;
	padTable_["�`"] = PAD_INPUT_1;

	keyTable_["�G�f�B�b�g"] = KEY_INPUT_F6;

	for (auto tblData : padTable_)			// �p�b�h����
	{
		data_.emplace(tblData.first, 0);
	}
	for (auto tblData : keyTable_)			// �L�[�{�[�h����
	{
		data_.emplace(tblData.first, 0);
	}

	return true;
}
