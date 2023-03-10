#include <DxLib.h>
#include "../ImageMng.h"
#include "ResultDrawInt.h"

// 描画の数(桁数)
constexpr int drawMax{ 4 };

ResultDrawInt::ResultDrawInt(int val,const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos) :
	ResultDrawValue{strImg,numImg,startTime,pos}
{
	drawMax_ = drawMax;
	// ゼロで初期化
	val_ = {0,0,0,0};
	constexpr int valMax{ 1000 };
	int tmp = valMax;

	// 一桁ずつに切り分ける
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

