#pragma once
#include <memory>
#include "Input.h"


class ClearAuto :
	public Input
{
public:
	ClearAuto();
	~ClearAuto();
private:
	bool Update(Obj& obj) override;
	bool Init(void);

	std::unique_ptr<Input> subInput_;
};

