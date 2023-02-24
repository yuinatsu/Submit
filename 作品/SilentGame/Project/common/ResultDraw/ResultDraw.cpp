#include "ResultDraw.h"

constexpr float aTime{ 0.5f };

ResultDraw::ResultDraw(float startTime, const Math::Vector2& pos) :
	drawStartTime_{ startTime },pos_{pos}, drawTime_{0.0f}
{
	// 描画も何もしないラムダ式入れとく
	drawFunc_ = []() {};
	updateFunc_ = std::bind(&ResultDraw::UpdateNon, this, std::placeholders::_1);
}

ResultDraw::~ResultDraw()
{
}

void ResultDraw::Update(const float& nowTime)
{
	updateFunc_(nowTime);
}

void ResultDraw::Draw(void)
{
	drawFunc_();
}

void ResultDraw::UpdateNon(const float& nowTime)
{
	if (nowTime >= drawStartTime_)
	{
		// 描画関数を切り替え
		drawFunc_ = std::bind(&ResultDraw::DrawOwn, this);

		// アップデート関数を切り替え
		updateFunc_ = std::bind(&ResultDraw::UpdateDraw,this,std::placeholders::_1);
	}
}

void ResultDraw::UpdateDraw(const float& nowTime)
{
	// 現在までの経過時間から描画開始の時間を引き描画時からの経過時間を求める
	drawTime_ = (nowTime - drawStartTime_);
}
