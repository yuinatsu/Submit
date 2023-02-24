#pragma once
#include "Input.h"


class Joypad :
	public Input
{
public:
	Joypad();
	~Joypad();
private:
	bool Update(Obj& obj) override;
	bool Init();

	std::map<std::string, int> padTable_;				// mapは基底でインクルード
	std::map<std::string, int> keyTable_;
	char padData_[256];									// データ格納
protected:
};

