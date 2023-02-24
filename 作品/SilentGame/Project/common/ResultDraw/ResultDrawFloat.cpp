#include <DxLib.h>
#include "../ImageMng.h"
#include "ResultDrawFloat.h"

// ­”“_ˆÈ‰º‚Ì•`‰æ”
constexpr int decimalPoint{ 2 };

// ‚·‚×‚Ä(.ŠÜ‚Ş)‚Ì•`‰æ”
constexpr int drawMax{ decimalPoint + 4 + 1 };


ResultDrawFloat::ResultDrawFloat(float val, const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos) :
	ResultDrawValue{strImg,numImg,startTime,pos}
{
	drawMax_ = drawMax;
	val_ = { 0,0,0,0,10,0,0 };
	

	
	int v = static_cast<int>(val);
	constexpr int valMax{ 1000 };
	int tmp = valMax;
	for (int i = 0; i < drawMax - (decimalPoint + 1); i++)
	{
		int num = (v / tmp) % 10;
		v -= num * tmp;
		val_[i] = num;
		tmp /= 10;
	}

	constexpr int dePointMax{100};
	tmp = dePointMax;
	v = static_cast<int>(val * dePointMax);
	for (int i = drawMax - decimalPoint; i < drawMax; i++)
	{
		int num = (v / tmp) % 10;
		v -= num * tmp;
		val_[i] = num;
		tmp /= 10;
	}
}

ResultDrawFloat::~ResultDrawFloat()
{
}

