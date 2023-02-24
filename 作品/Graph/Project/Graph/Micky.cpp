#include <dxlib.h>
#include "Micky.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Micky::Micky(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	color_ = 0xffff00;                                          // 初期カラー
}

Micky::~Micky()
{
}

void Micky::Update(float delta, ShapeVec& shapeVec)
{
}

void Micky::Draw(void)
{
	DrawCircle((int)(pos_.x + size_.x), (int)(pos_.y - size_.x), (int)(size_.x * 2 / 3), color_, true);		// 右耳
	DrawCircle((int)(pos_.x - size_.x), (int)(pos_.y - size_.x), (int)(size_.x * 2 / 3), color_, true);		// 左耳
	DrawCircle((int)pos_.x, (int)pos_.y, (int)size_.x, color_, true);		// 顔
	DrawBox((int)(pos_.x - size_.x * 3 / 2), (int)(pos_.y - size_.x / 4), (int)(pos_.x + size_.x * 3 / 2), (int)(pos_.y + size_.x / 4), 0xffffff, true);		// モザイク
}

void Micky::Draw(float num)
{
}
