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

	std::map<std::string, int> padTable_;				// map�͊��ŃC���N���[�h
	std::map<std::string, int> keyTable_;
	char padData_[256];									// �f�[�^�i�[
protected:
};

