#include <DxLib.h>
#include "../ImageMng.h"
#include "ResultDrawValue.h"


constexpr int numStrSize{ 50 };

ResultDrawValue::ResultDrawValue(const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos) :
	ResultDraw{startTime,pos}, strImgName_{strImg}, numImgName_{numImg}
{
	GetGraphSize(lpImageMng.GetID(strImgName_)[0], &imgSize_.x, &imgSize_.y);

	imgPos_ = pos_;
	imgPos_.x -= imgSize_.x / 2 + numStrSize / 2;
}

ResultDrawValue::~ResultDrawValue()
{
}

void ResultDrawValue::DrawOwn(void)
{
	int alpha{ std::clamp(static_cast<int>((drawTime_ / fadeTime_) * 255.0f),0,255) };
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawRotaGraph(static_cast<int>(imgPos_.x), static_cast<int>(imgPos_.y), 1.0, 0.0, lpImageMng.GetID(strImgName_)[0], true);
	const auto tmp = static_cast<int>(pos_.x) + (numStrSize / 2);
	for (int i = 0; i < drawMax_; i++)
	{
		DrawRotaGraph(
			tmp + i * numStrSize,
			static_cast<int>(pos_.y),
			1.0,
			0.0,
			lpImageMng.GetID(numImgName_)[val_[i]],
			true
		);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

