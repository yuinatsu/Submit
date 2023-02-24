#include "Singleton.h"
#include <DxLib.h>

void Singleton::OutputLife()
{
	char str[32];
	sprintf_s(str, 32, "life=%d", life_);
	OutputDebugStringA(str);
	//DrawFormatString(480, 10, 0xaaaaff, L"life=%d", life_);
	++life_;
}
