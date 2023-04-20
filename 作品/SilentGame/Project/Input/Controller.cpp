#include "Controller.h"
#include <DxLib.h>
#include <time.h>
#include "../common/Debug.h"

Controller::Controller()
{
	data_.fill(std::make_pair(false, false));
}

Controller::~Controller()
{
}

const inputData& Controller::Get(void)
{
	return data_;
}

bool Controller::Press(InputID id)
{
	return data_[static_cast<size_t>(id)].first;
}

bool Controller::Pressed(InputID id)
{
	return data_[static_cast<size_t>(id)].first && !data_[static_cast<size_t>(id)].second;
}

bool Controller::Released(InputID id)
{
	return !data_[static_cast<size_t>(id)].first && data_[static_cast<size_t>(id)].second;
}

bool Controller::NotPress(InputID id)
{
	return !data_[static_cast<size_t>(id)].first;
}

bool Controller::MovePress(bool isMove)
{
	bool rtnflag = false;
	for (int n = static_cast<int>(InputID::Up); n <= static_cast<int>(InputID::Right); n++)
	{
		rtnflag |= data_[static_cast<size_t>(static_cast<InputID>(n))].first;
	}
	return isMove == true ? rtnflag : !rtnflag;
}

bool Controller::MoveReleased()
{
	bool rtnflag = false;
	for (int n = static_cast<int>(InputID::Up); n <= static_cast<int>(InputID::Right); n++)
	{
		rtnflag |= !data_[static_cast<size_t>(static_cast<InputID>(n))].first && data_[static_cast<size_t>(static_cast<InputID>(n))].second;
	}
	return rtnflag;
}

bool Controller::MovePressed()
{
	bool rtnflag = false;
	for (int n = static_cast<int>(InputID::Up); n <= static_cast<int>(InputID::Right); n++)
	{
		rtnflag |= data_[static_cast<size_t>(static_cast<InputID>(n))].first && !data_[static_cast<size_t>(static_cast<InputID>(n))].second;
	}
	return rtnflag;
}

bool Controller::WalkPress()
{
	bool rtnflag = false, flag1 = false, flag2 = false;
	for (int n = static_cast<int>(InputID::Up); n <= static_cast<int>(InputID::Right); n++)
	{
		flag1 |= data_[static_cast<size_t>(static_cast<InputID>(n))].first;
	}
	for (int n = static_cast<int>(InputID::Dash); n <= static_cast<int>(InputID::Crouch); n++)
	{
		flag2 |= data_[static_cast<size_t>(static_cast<InputID>(n))].first;
	}
	return (flag1 && !flag2);
}

bool Controller::IsNeutral()
{
	bool rtnflag = false;
	for (int n = 0; n < static_cast<int>(InputID::Max); n++)
	{
		rtnflag |= data_[static_cast<size_t>(static_cast<InputID>(n))].first;
	}
	return !rtnflag;
}

bool Controller::IsAnyPress()
{
	bool rtnflag = false;
	for (int n = 0; n < static_cast<int>(InputID::Max); n++)
	{
		rtnflag |= data_[static_cast<size_t>(static_cast<InputID>(n))].first && !data_[static_cast<size_t>(static_cast<InputID>(n))].second;
	}
	return !rtnflag;
}
