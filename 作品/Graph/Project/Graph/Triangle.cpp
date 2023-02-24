#include <dxlib.h>
#include "Triangle.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Triangle::Triangle(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)
{
	moveLR_ = MoveLR::Left;										// �������E�ړ�����
	moveUD_ = MoveUD::Up;										// �����㉺�ړ�����
	color_ = 0xffff00;                                          // �����J���[
	speed_ = 250;                                               // �ړ��X�s�[�h
}

Triangle::~Triangle()
{
}

void Triangle::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y + size_.y, pos_.y, pos_.x - size_.x / 2, pos_.x + size_.x / 2);			// �ړ������X�V
	if (moveLR_ == MoveLR::Left)
	{
		pos_.x -= speed_ * delta;				// ���ړ�
	}
	else
	{
		pos_.x += speed_ * delta;				// �E�ړ�
	}
	if (moveUD_ == MoveUD::Up)
	{
		pos_.y -= speed_ / 3.0f * delta;		// ��ړ�
	}
	else
	{
		pos_.y += speed_ / 3.0f * delta;		// ���ړ�
	}

	// ���x����
	if (speed_ < 1)
	{
		speed_ = 0;
	}

	// �����蔻��
	for (auto& shape : shapeVec)
	{
		// �O�p�`�Ƌ�`
		if (shape->GetType() == ShapeType::Square)
		{
			// �����������̏���
			if (IsHitSquareAndTriangle(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				speed_ += 10;					// ����
			}
		}
		// �O�p�`�Ɖ~
		if (shape->GetType() == ShapeType::Circle)
		{
			if (IsHitCircleAndTriangle(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				speed_ -= 10;					// ����
			}
		}
		// �O�p�`�ƃc���[
		if (shape->GetType() == ShapeType::Tree)
		{
			// �����������̏���
			if (IsHitTriangleAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ += 10;					// ����
			}
		}
		// �O�p�`�ƃ~�b�Z�[
		if (shape->GetType() == ShapeType::Micky)
		{
			if (IsHitTriangleAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ -= 10;					// ����

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// �O�p�`�Ɖ�
		if (shape->GetType() == ShapeType::Star)
		{
			if (IsHitTriangleAndStar(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				speed_ += 10;					// ����
				color_ = shape->GetColor();
			}
		}
	}
}


void Triangle::Draw(void)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - size_.x / 2), (int)(pos_.y + size_.y), (int)(pos_.x + size_.x / 2), (int)(pos_.y + size_.y), color_, true);			// �O�p�`��
}


void Triangle::Draw(float num)
{
	DrawTriangle((int)pos_.x, (int)pos_.y, (int)(pos_.x - (size_.x * num) / 2), (int)(pos_.y + size_.y * num), (int)(pos_.x + (size_.x * num) / 2), (int)(pos_.y + size_.y * num), color_, true);			// �O�p�`��
}
