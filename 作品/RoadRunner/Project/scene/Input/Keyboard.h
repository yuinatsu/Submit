#pragma once
#include "Input.h"


class Keyboard :
	public Input
{
public:
	Keyboard();
	~Keyboard();
private:
	bool Update(Obj& obj) override;
	bool Init();
protected:
	char keyData_[256];									// データ格納
	std::map<std::string, int> keyTable_;				// mapは基底でインクルード
};

