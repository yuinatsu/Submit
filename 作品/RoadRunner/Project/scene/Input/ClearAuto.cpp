//#include <DxLib.h>
#include "ClearAuto.h"
#include "InputSelect.h"
#include "../Obj/MoveCtl.h"


ClearAuto::ClearAuto()
{
	Init();
}


ClearAuto::~ClearAuto()
{
}


bool ClearAuto::Update(Obj& obj)
{
	for (auto& data : data_)			// ���͏��NOW��OLD���̏��擾
	{
		dataOld_[data.first] = data.second;
	}


	if (CheckLad()(obj, DIR::UP))					// �͂��������I����܂ŏ�L�[�̓��͂���������
	{
		data_["��"] = 1;
	}
	else
	{
		data_["��"] = 0;
		subInput_->Update(obj);

		auto& inputData = subInput_->GetData(InputType::NOW);
		if (inputData.count("�`"))
		{
			data_["�G�f�B�b�g"] = inputData.at("�`");
		}
	}

	return true;
}


bool ClearAuto::Init(void)
{
	data_["��"] = 0;
	data_["��"] = 0;
	data_["��"] = 0;
	data_["�E"] = 0;
	data_["�`"] = 0;

	subInput_ = InputSelect()();

	return true;
}
