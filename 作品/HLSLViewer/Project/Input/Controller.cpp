#include "Controller.h"
#include <DxLib.h>
#include <time.h>

Controller::Controller()
{
	data_.fill(std::make_pair(false, false));
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
