#include "Button.h"
#include <DxLib.h>
#include "../Common/ResourceMng.h"
#include "../Application.h"

Button::Button(PushFunc&& pushFunc, const std::filesystem::path& btnPath, const std::filesystem::path& hitCursorPath, const Vector2& pos) :
	ButtonBase{pos}, pushFunc_{pushFunc}
{
	draw_ = &Button::DrawNotHitCursor;
	lpResourceMng.LoadTexture(btnHandle_, btnPath);
	lpResourceMng.LoadTexture(hitCursorHandle_, hitCursorPath);
}

void Button::Update(float delta, ObjectManager& objMng, Controller& controller)
{
}

void Button::Draw()
{
	(this->*draw_)();
}

void Button::Push(BaseScene& scene, Controller& controller)
{
	pushFunc_(scene);
}

void Button::HitCursor(void)
{
	draw_ = &Button::DrawHitCursor;
	isHitCursor_ = true;
}

void Button::NotHitCursor(void)
{
	draw_ = &Button::DrawNotHitCursor;
	isHitCursor_ = false;
}

void Button::Loaded(void)
{
	GetGraphSizeF(*btnHandle_, &size_.x, &size_.y);
	pos_ -= size_ / 2.0f;
}

void Button::DrawHitCursor(void)
{
	DrawGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), *btnHandle_, true);
	DrawGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), *hitCursorHandle_, true);}


void Button::DrawNotHitCursor(void)
{
	DrawGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), *btnHandle_, true);
}
