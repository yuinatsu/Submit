#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

const InputData& Input::GetData(InputType type)
{
	return (type == InputType::NOW) ? data_ : dataOld_;
}
