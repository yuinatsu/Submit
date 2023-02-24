#include "Input.h"


Input::Input()
{
}


Input::~Input()
{
}


bool Input::Update(Obj& obj)
{
	return false;
}

bool Input::Update(Obj& obj, Obj& other)
{
	return false;
}

const InputData& Input::GetData(InputType type)
{
	if (type == InputType::NOW)
	{
		return data_;
	}
	else
	{
		return dataOld_;
	}
}

