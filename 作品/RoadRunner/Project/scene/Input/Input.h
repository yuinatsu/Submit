#pragma once
#include <map>
#include <string>

enum class InputType
{
	NOW,				// Œ»İ‚Ì“ü—Íî•ñ
	OLD,				// ‚P‚Â‘O‚Ì“ü—Íî•ñ
};

using InputData = std::map<std::string, int>;

class Obj;

class Input
{
public:
	Input();
	virtual ~Input();
	virtual bool Update(Obj& obj) = 0;				// Œp³æ‚Å‚Ì‚İg—p
	virtual bool Update(Obj& obj, Obj& other);
	const InputData& GetData(InputType type);
private:

protected:
	InputData data_;
	InputData dataOld_;
};

