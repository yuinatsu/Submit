#include <dxlib.h>
#include "Circle.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Circle::Circle(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;											// 初期左右移動向き
	moveUD_ = MoveUD::Up;											// 初期上下移動向き
	int color_ = 0x00ffff;                                          // 初期カラー
	int speed_ = 300;                                               // 移動スピード
}

Circle::~Circle()
{
}

void Circle::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y - size_.y, pos_.y + size_.y, pos_.x - size_.x, pos_.x + size_.x);
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;		// 左移動
	}
	else
	{
		pos_.x += speed_ * delta;		// 右移動
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ * delta;		// 上移動
	}
	else
	{
		pos_.y += speed_ * delta;		// 下移動
	}
	if (hitWall_ != WallDir::Max)
	{
		size_ -= Vector2Flt{ 2,2 };		// 縮小
	}

	if (size_.x <= 10)
	{
		size_ = Vector2Flt{ 10,10 };	// 最低値
	}

	// 当たり判定
	for (auto& shape : shapeVec)
	{
		// 円と矩形
		if (shape->GetType() == ShapeType::Square)
		{
			// 当たった時の処理
			if (IsHitCircleAndSquare(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ -= Vector2Flt{ 1,1 };
			}
		}
		// 円と三角形
		if (shape->GetType() == ShapeType::Triangle)
		{
			// 当たった時の処理
			if (IsHitCircleAndTriangle(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ += Vector2Flt{ 1,1 };
			}
		}
		// 円とツリー
		if (shape->GetType() == ShapeType::Tree)
		{
			// 当たった時の処理
			if (IsHitCircleAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ += Vector2Flt{ 1,1 };
			}
		}
		// 円とミッ〇ー
		if (shape->GetType() == ShapeType::Micky)
		{
			// 当たった時の処理
			if (IsHitCircleAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ -= Vector2Flt{ 1,1 };

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// 円と家
		if (shape->GetType() == ShapeType::Star)
		{
			if (IsHitCircleAndStar(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ -= Vector2Flt{ 1,1 };

				color_ = shape->GetColor();
			}
		}
	}
}

void Circle::Draw(void)
{
	DrawCircle((int)pos_.x, (int)pos_.y, (int)size_.x, color_, true);			// 円描画
}

void Circle::Draw(float num)
{
	DrawCircle((int)pos_.x, (int)pos_.y, (int)(size_.x * num), color_, true);			// 円描画
}
