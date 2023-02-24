#pragma once
#include <map>
#include <string>

enum class InputType
{
	NOW,				// ���݂̓��͏��
	OLD,				// �P�O�̓��͏��
};

using InputData = std::map<std::string, int>;

class Obj;

class Input
{
public:
	Input();
	virtual ~Input();
	virtual bool Update(Obj& obj) = 0;				// �p����ł̂ݎg�p
	virtual bool Update(Obj& obj, Obj& other);
	const InputData& GetData(InputType type);
private:

protected:
	InputData data_;
	InputData dataOld_;
};

