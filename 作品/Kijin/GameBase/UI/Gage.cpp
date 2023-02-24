#include "Gage.h"
#include <DxLib.h>
#include "../Common/ResourceMng.h"
#include "../Application.h"

Gage::Gage(DataBindFunc&& dataBindFunc, const std::filesystem::path& gagePath, const std::filesystem::path& framePath, float defalutValue, const Vector2& pos) :
	UiBase{pos}, dataBindFunc_{dataBindFunc}, value_{ defalutValue }
{
	lpResourceMng.LoadTexture(gageHandle_, gagePath);
	lpResourceMng.LoadTexture(frameHandle_, framePath);
}

void Gage::Update(float delta, ObjectManager& objMng, Controller& controller)
{
	value_ = Clamp(dataBindFunc_(objMng) / size_.x);
}

void Gage::Draw()
{
	DrawRectGraphF(pos_.x, pos_.y, 2, 2, static_cast<int>(size_.x * value_) + 2, static_cast<int>(size_.y) + 2, *gageHandle_, true);
	DrawGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), *frameHandle_, true);
}

void Gage::Loaded(void)
{
	GetGraphSizeF(*gageHandle_, &size_.x, &size_.y);
}
