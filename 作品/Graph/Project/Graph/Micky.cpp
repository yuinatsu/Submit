#include <dxlib.h>
#include "Micky.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Micky::Micky(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	color_ = 0xffff00;                                          // �����J���[
}

Micky::~Micky()
{
}

void Micky::Update(float delta, ShapeVec& shapeVec)
{
}

void Micky::Draw(void)
{
	DrawCircle((int)(pos_.x + size_.x), (int)(pos_.y - size_.x), (int)(size_.x * 2 / 3), color_, true);		// �E��
	DrawCircle((int)(pos_.x - size_.x), (int)(pos_.y - size_.x), (int)(size_.x * 2 / 3), color_, true);		// ����
	DrawCircle((int)pos_.x, (int)pos_.y, (int)size_.x, color_, true);		// ��
	DrawBox((int)(pos_.x - size_.x * 3 / 2), (int)(pos_.y - size_.x / 4), (int)(pos_.x + size_.x * 3 / 2), (int)(pos_.y + size_.x / 4), 0xffffff, true);		// ���U�C�N
}

void Micky::Draw(float num)
{
}
