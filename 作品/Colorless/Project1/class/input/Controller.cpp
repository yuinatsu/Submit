#include "Controller.h"

Controller::~Controller()
{
}

bool Controller::Press(InputID id)
{
	return cntData_[id][static_cast<int>(Trg::Now)];
}

bool Controller::Pressed(InputID id)
{
	return cntData_[id][static_cast<int>(Trg::Now)] && !cntData_[id][static_cast<int>(Trg::Old)];
}

bool Controller::Released(InputID id)
{
	return !cntData_[id][static_cast<int>(Trg::Now)] && cntData_[id][static_cast<int>(Trg::Old)];
}

bool Controller::SpPress(InputID id)
{
	if (!cntData_[id][static_cast<int>(Trg::Now)]) 
	{
		return false;
	}
	for (auto cntid : cntData_) 
	{
		if (id != cntid.first) 
		{
			if (cntData_[cntid.first][static_cast<int>(Trg::Now)]) 
			{
				return false;
			};
		}
	}
	return true;
}
