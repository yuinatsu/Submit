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
	for (auto& data : data_)			// ���͏��NOW��OLD���̏��擾
	{
		dataOld_[data.first] = data.second;
	}
	GetHitKeyStateAll(keyData_);
	for (auto tblData : keyTable_)
	{
		data_[tblData.first] = keyData_[tblData.second];				// ��jdata["��"] = keyData_[keyTable_["��"]];
	}

	return true;
}


bool Keyboard::Init()
{
	keyTable_["��"] = KEY_INPUT_UP;
	keyTable_["��"] = KEY_INPUT_DOWN;
	keyTable_["��"] = KEY_INPUT_LEFT;
	keyTable_["�E"] = KEY_INPUT_RIGHT;
	keyTable_["�`"] = KEY_INPUT_Z;
	keyTable_["�G�f�B�b�g"] = KEY_INPUT_F6;


	for (auto tblData : keyTable_)			//
	{
		data_.emplace(tblData.first, 0);
	}

	// �L�[�R���t�B�O

	return true;
}
