#include <dxlib.h>
#include "Triangle.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Triangle::Triangle(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;										// 初期左右移動向き
	moveUD_ = MoveUD::Up;										// 初期上下移動向き
	color_ = 0xffff00;                                          // 初期カラー
	speed_ = 250;                                               // 移動スピード
}

Triangle::~Triangle()
{
}

void Triangle::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y + size_.y, pos_.y, pos_.x - size_.x / 2, pos_.x + size_.x / 2);			// 移動方向更新
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;				// 左移動
	}
	else
	{
		pos_.x += speed_ * delta;				// 右移動
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ / 3.0f * delta;		// 上移動
	}
	else
	{
		pos_.y += speed_ / 3.0f * delta;		// 下移動
	}

	// 速度下限
	if (speed_ < 1)
	{
		speed_ = 0;
	}

	// 当たり判定
	for (auto& shape : shapeVec)
	{
		// 三角形と矩形
		if (shape->GetType() == ShapeType::Square)
		{
			// 当たった時の処理
			if (IsHitSquareAndTriangle(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				speed_ += 10;					// 加速
			}
		}
		// 三角形と円
		if (shape->GetType() == ShapeType::Circle)
		{
			if (IsHitCircleAndTriangle(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				speed_ -= 10;					// 減速
			}
		}
		// 三角形とツリー
		if (shape->GetType() == ShapeType::Tree)
		{
			// 当たった時の処理
			if (IsHitTriangleAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ += 10;					// 加速
			}
		}
		// 三角形とミッ〇ー
		if (shape->GetType() == ShapeType::Micky)
		{
			if (IsHitTriangleAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ -= 10;					// 減速

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// 三角形と家
		if (shape->GetType() == ShapeType::Star)
		{
			if (IsHitTriangleAndStar(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ += 10;					// 加速
				color_ = shape->GetColor();
			}
		}
	}
}


void Triangle::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x / 2), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x / 2), (int)(pos_.y + size_.y), color_, true);			// 三角描画
}


void Triangle::Draw(float num)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - (size_.x * num) / 2), (int)(pos_.y + size_.y * num), (int)(pos_.x + (size_.x * num) / 2), (int)(pos_.y + size_.y * num), color_, true);			// 三角描画
}
