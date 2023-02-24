#include <dxlib.h>
#include "Star.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Star::Star(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Right;									// 初期左右移動向き
	moveUD_ = MoveUD::Down;										// 初期上下移動向き
	color_ = 0x00ff00;                                          // 初期カラー
	speed_ = 300;                                               // 移動スピード
}

Star::~Star()
{
}

void Star::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y - size_.y, pos_.y + size_.y, pos_.x - size_.x, pos_.x + size_.x);			// 移動方向更新
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;			// 左移動
	}
	else
	{
		pos_.x += speed_ * delta;			// 右移動
	}
	if (hitWall_ != WallDir::Max)
	{
		if (hitWall_ == WallDir::Left)
		{
			color_ = 0xff0000;			// 左の壁に触れたら赤色に
		}
		else if (hitWall_ == WallDir::Right)
		{
			color_ = 0x0000ff;			// 右の壁に触れたら緑色に
		}
	}
}

void Star::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x), (int)(pos_.y + size_.y), color_, true);			// 屋根描画
	DrawBox((int)(pos_.x - size_.x * 2 / 3), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x * 2 / 3), (int)(pos_.y + size_.y * 2), color_, true);				// 四角描画
}

void Star::Draw(float num)
{
}
