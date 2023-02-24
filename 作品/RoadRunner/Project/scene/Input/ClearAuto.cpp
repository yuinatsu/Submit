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
	for (auto& data : data_)			// 入力情報がNOWかOLDかの情報取得
	{
		dataOld_[data.first] = data.second;
	}


	if (CheckLad()(obj, DIR::UP))					// はしごを上り終えるまで上キーの入力をし続ける
	{
		data_["上"] = 1;
	}
	else
	{
		data_["上"] = 0;
		subInput_->Update(obj);

		auto& inputData = subInput_->GetData(InputType::NOW);
		if (inputData.count("Ａ"))
		{
			data_["エディット"] = inputData.at("Ａ");
		}
	}

	return true;
}


bool ClearAuto::Init(void)
{
	data_["上"] = 0;
	data_["下"] = 0;
	data_["左"] = 0;
	data_["右"] = 0;
	data_["Ａ"] = 0;

	subInput_ = InputSelect()();

	return true;
}
