#include <dxlib.h>
#include "Circle.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Circle::Circle(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;											// �������E�ړ�����
	moveUD_ = MoveUD::Up;											// �����㉺�ړ�����
	int color_ = 0x00ffff;                                          // �����J���[
	int speed_ = 300;                                               // �ړ��X�s�[�h
}

Circle::~Circle()
{
}

void Circle::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y - size_.y, pos_.y + size_.y, pos_.x - size_.x, pos_.x + size_.x);
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;		// ���ړ�
	}
	else
	{
		pos_.x += speed_ * delta;		// �E�ړ�
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ * delta;		// ��ړ�
	}
	else
	{
		pos_.y += speed_ * delta;		// ���ړ�
	}
	if (hitWall_ != WallDir::Max)
	{
		size_ -= Vector2Flt{ 2,2 };		// �k��
	}

	if (size_.x <= 10)
	{
		size_ = Vector2Flt{ 10,10 };	// �Œ�l
	}

	// �����蔻��
	for (auto& shape : shapeVec)
	{
		// �~�Ƌ�`
		if (shape->GetType() == ShapeType::Square)
		{
			// �����������̏���
			if (IsHitCircleAndSquare(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ -= Vector2Flt{ 1,1 };
			}
		}
		// �~�ƎO�p�`
		if (shape->GetType() == ShapeType::Triangle)
		{
			// �����������̏���
			if (IsHitCircleAndTriangle(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ += Vector2Flt{ 1,1 };
			}
		}
		// �~�ƃc���[
		if (shape->GetType() == ShapeType::Tree)
		{
			// �����������̏���
			if (IsHitCircleAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ += Vector2Flt{ 1,1 };
			}
		}
		// �~�ƃ~�b�Z�[
		if (shape->GetType() == ShapeType::Micky)
		{
			// �����������̏���
			if (IsHitCircleAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				size_ -= Vector2Flt{ 1,1 };

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// �~�Ɖ�
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
	DrawCircle((int)pos_.x, (int)pos_.y, (int)size_.x, color_, true);			// �~�`��
}

void Circle::Draw(float num)
{
	DrawCircle((int)pos_.x, (int)pos_.y, (int)(size_.x * num), color_, true);			// �~�`��
}
