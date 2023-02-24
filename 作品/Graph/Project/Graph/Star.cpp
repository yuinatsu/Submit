#include <dxlib.h>
#include "Star.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Star::Star(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Right;									// �������E�ړ�����
	moveUD_ = MoveUD::Down;										// �����㉺�ړ�����
	color_ = 0x00ff00;                                          // �����J���[
	speed_ = 300;                                               // �ړ��X�s�[�h
}

Star::~Star()
{
}

void Star::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y - size_.y, pos_.y + size_.y, pos_.x - size_.x, pos_.x + size_.x);			// �ړ������X�V
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;			// ���ړ�
	}
	else
	{
		pos_.x += speed_ * delta;			// �E�ړ�
	}
	if (hitWall_ != WallDir::Max)
	{
		if (hitWall_ == WallDir::Left)
		{
			color_ = 0xff0000;			// ���̕ǂɐG�ꂽ��ԐF��
		}
		else if (hitWall_ == WallDir::Right)
		{
			color_ = 0x0000ff;			// �E�̕ǂɐG�ꂽ��ΐF��
		}
	}
}

void Star::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x), (int)(pos_.y + size_.y), color_, true);			// �����`��
	DrawBox((int)(pos_.x - size_.x * 2 / 3), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x * 2 / 3), (int)(pos_.y + size_.y * 2), color_, true);				// �l�p�`��
}

void Star::Draw(float num)
{
}
