#include <dxlib.h>
#include "Tree.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Tree::Tree(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;										// �������E�ړ�����
	moveUD_ = MoveUD::Down;										// �����㉺�ړ�����
	color_ = 0x00ff00;                                          // �����J���[
	speed_ = 100;                                               // �ړ��X�s�[�h
}

Tree::~Tree()
{
}

void Tree::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y, pos_.y + (size_.y + 15) * 2, pos_.x - size_.x / 2, pos_.x + size_.x / 2);			// �ړ������X�V
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;			// ���ړ�
	}
	else
	{
		pos_.x += speed_ * delta;			// �E�ړ�
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ * delta;			// ��ړ�
	}
	else
	{
		pos_.y += speed_ * delta;			// ���ړ�
	}
}

void Tree::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - (size_.x + size_.x / 3) / 2), (int)(pos_.y + size_.y * 2), (int)(pos_.x + (size_.x + size_.x / 3) / 2), (int)(pos_.y + size_.y * 2), color_, true);		// �㕔�O�p�`��
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x / 2), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x / 2), (int)(pos_.y + size_.y), color_, true);			// �����O�p�`��
	DrawBox((int)(pos_.x - size_.x / 3), (int)(pos_.y + size_.y * 2), (int)(pos_.x + size_.x / 3), (int)(pos_.y + (size_.y + 15) * 2), color_, true);			// �ۑ��`��
}

void Tree::Draw(float num)
{
}
