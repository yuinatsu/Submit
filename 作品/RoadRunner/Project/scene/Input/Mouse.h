#pragma once
#include "Input.h"
#include "../../common/Vector2.h"


class Mouse :
	public Input
{
public:
	Mouse();
	Mouse(Vector2 offset);
	~Mouse();
private:
	bool Update(Obj& obj) override;
	bool Init();
	Vector2 offset_;
protected:
};

