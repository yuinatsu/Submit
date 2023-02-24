#include <DxLib.h>
#include "../ImageMng.h"
#include "ResultDrawInt.h"

// •`‰æ‚Ì”(Œ…”)
constexpr int drawMax{ 4 };

ResultDrawInt::ResultDrawInt(int val,const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos) :
	ResultDrawValue{strImg,numImg,startTime,pos}
{
	drawMax_ = drawMax;
	// ƒ[ƒ‚Å‰Šú‰»
	val_ = {0,0,0,0};
	constexpr int valMax{ 1000 };
	int tmp = valMax;

	// ˆêŒ…‚¸‚Â‚ÉØ‚è•ª‚¯‚é
	for (int i = 0; i < drawMax; i++)
	{
		int num = (val / tmp)%10;
		val -= num * tmp;
		val_[i] = num;
		tmp /= 10;
	}

}

ResultDrawInt::~ResultDrawInt()
{
}

