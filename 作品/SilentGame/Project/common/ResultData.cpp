#include "ResultData.h"

ResultData::ResultData() :
	killCount_{0}, alertCount_{0}, clearTime_{0.0f}, isClear_{true}, itemUseCount_{0}
{
}

ResultData::~ResultData()
{
}

void ResultData::AddKillCount(int val)
{
	killCount_ += val;
}


void ResultData::CountUpAlertCount(void)
{
	alertCount_++;
}

void ResultData::UpdateClearTime(float delta)
{
	clearTime_ += delta;
}

void ResultData::SetClear(bool flag)
{
	isClear_ = flag;
}

void ResultData::CountUpItemUseCount(void)
{
	itemUseCount_++;
}
