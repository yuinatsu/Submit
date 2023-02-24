#include <dxlib.h>
#include "Square.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Square::Square(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)// コンスト付き変数はコンストラクタが走る前に記述
{
	moveLR_ = MoveLR::Right;									// 初期左右移動向き
	moveUD_ = MoveUD::Up;										// 初期上下移動向き
	color_ = 0x00ff00;                                          // 初期カラー
	speed_ = 200;                                               // 移動スピード
}

Square::~Square()
{
}

void Square::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y, pos_.y + size_.y, pos_.x, pos_.x + size_.x);
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;			// 左移動
	}
	else
	{
		pos_.x += speed_ * delta;			// 右移動
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ * 2 * delta;		// 上移動
	}
	else
	{
		pos_.y += speed_ * 2 * delta;		// 下移動
	}

	// 当たり判定
	for (auto& shape : shapeVec)
	{
		// 矩形と円
		if (shape->GetType() == ShapeType::Circle)
		{
			// 当たった時の処理
			if (IsHitCircleAndSquare(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				color_ = 0x00ffff;
			}
		}
		// 矩形と三角
		if (shape->GetType() == ShapeType::Triangle)
		{
			// 当たった時の処理
			if (IsHitSquareAndTriangle(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0xff00ff;
			}
		}
		// 矩形とツリー
		if (shape->GetType() == ShapeType::Tree)
		{
			// 当たった時の処理
			if (IsHitSquareAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0xffff00;
			}
		}
		// 矩形とミッ〇ー
		if (shape->GetType() == ShapeType::Micky)
		{
			// 当たった時の処理
			if (IsHitSquareAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0x0000ff;

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// 矩形と家
		if (shape->GetType() == ShapeType::Star)
		{
			// 当たった時の処理
			if (IsHitSquareAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = shape->GetColor();
			}
		}
	}
}

void Square::Draw(void)
{
	DrawBox((int)pos_.x, (int)pos_.y, (int)(pos_.x + size_.x), (int)(pos_.y + size_.y), color_, true);			// 矩形描画
}

void Square::Draw(float num)
{
	DrawBox((int)pos_.x, (int)pos_.y, (int)(pos_.x + size_.x * num), (int)(pos_.y + size_.y * num), color_, true);		// 矩形描画
}

void Square::Draw(float num, Vector2Flt offset)
{
	Vector2Flt pos{ pos_ + offset - size_ * num * offset / size_ };			// オフセット値と倍率計算
	DrawBox((int)pos.x, (int)pos.y, (int)(pos.x + size_.x * num), (int)(pos.y + size_.y * num), color_, true);			// 矩形描画
}
