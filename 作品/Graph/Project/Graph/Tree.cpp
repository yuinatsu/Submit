#include <dxlib.h>
#include "Tree.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Tree::Tree(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;										// 初期左右移動向き
	moveUD_ = MoveUD::Down;										// 初期上下移動向き
	color_ = 0x00ff00;                                          // 初期カラー
	speed_ = 100;                                               // 移動スピード
}

Tree::~Tree()
{
}

void Tree::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y, pos_.y + (size_.y + 15) * 2, pos_.x - size_.x / 2, pos_.x + size_.x / 2);			// 移動方向更新
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
		pos_.y -= speed_ * delta;			// 上移動
	}
	else
	{
		pos_.y += speed_ * delta;			// 下移動
	}
}

void Tree::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - (size_.x + size_.x / 3) / 2), (int)(pos_.y + size_.y * 2), (int)(pos_.x + (size_.x + size_.x / 3) / 2), (int)(pos_.y + size_.y * 2), color_, true);		// 上部三角描画
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x / 2), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x / 2), (int)(pos_.y + size_.y), color_, true);			// 下部三角描画
	DrawBox((int)(pos_.x - size_.x / 3), (int)(pos_.y + size_.y * 2), (int)(pos_.x + size_.x / 3), (int)(pos_.y + (size_.y + 15) * 2), color_, true);			// 丸太描画
}

void Tree::Draw(float num)
{
}
