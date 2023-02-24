#include <dxlib.h>
#include "Square.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Square::Square(Vector2Flt&& pos, Vector2Flt&& size) : Shape(pos, size)// �R���X�g�t���ϐ��̓R���X�g���N�^������O�ɋL�q
{
	moveLR_ = MoveLR::Right;									// �������E�ړ�����
	moveUD_ = MoveUD::Up;										// �����㉺�ړ�����
	color_ = 0x00ff00;                                          // �����J���[
	speed_ = 200;                                               // �ړ��X�s�[�h
}

Square::~Square()
{
}

void Square::Update(float delta, ShapeVec& shapeVec)
{
	UpdateMoveDir(pos_.y, pos_.y + size_.y, pos_.x, pos_.x + size_.x);
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
		pos_.y -= speed_ * 2 * delta;		// ��ړ�
	}
	else
	{
		pos_.y += speed_ * 2 * delta;		// ���ړ�
	}

	// �����蔻��
	for (auto& shape : shapeVec)
	{
		// ��`�Ɖ~
		if (shape->GetType() == ShapeType::Circle)
		{
			// �����������̏���
			if (IsHitCircleAndSquare(shape->GetPos(), shape->GetSize(), pos_, size_))
			{
				color_ = 0x00ffff;
			}
		}
		// ��`�ƎO�p
		if (shape->GetType() == ShapeType::Triangle)
		{
			// �����������̏���
			if (IsHitSquareAndTriangle(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0xff00ff;
			}
		}
		// ��`�ƃc���[
		if (shape->GetType() == ShapeType::Tree)
		{
			// �����������̏���
			if (IsHitSquareAndTree(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0xffff00;
			}
		}
		// ��`�ƃ~�b�Z�[
		if (shape->GetType() == ShapeType::Micky)
		{
			// �����������̏���
			if (IsHitSquareAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = 0x0000ff;

				auto scrSize = Vector2Flt{ 1024 - size_.x,768 - size_.y };
				pos_.x = rand() % (int)scrSize.x;
				pos_.y = rand() % (int)scrSize.y;
			}
		}
		// ��`�Ɖ�
		if (shape->GetType() == ShapeType::Star)
		{
			// �����������̏���
			if (IsHitSquareAndMicky(pos_, size_, shape->GetPos(), shape->GetSize()))
			{
				color_ = shape->GetColor();
			}
		}
	}
}

void Square::Draw(void)
{
	DrawBox((int)pos_.x, (int)pos_.y, (int)(pos_.x + size_.x), (int)(pos_.y + size_.y), color_, true);			// ��`�`��
}

void Square::Draw(float num)
{
	DrawBox((int)pos_.x, (int)pos_.y, (int)(pos_.x + size_.x * num), (int)(pos_.y + size_.y * num), color_, true);		// ��`�`��
}

void Square::Draw(float num, Vector2Flt offset)
{
	Vector2Flt pos{ pos_ + offset - size_ * num * offset / size_ };			// �I�t�Z�b�g�l�Ɣ{���v�Z
	DrawBox((int)pos.x, (int)pos.y, (int)(pos.x + size_.x * num), (int)(pos.y + size_.y * num), color_, true);			// ��`�`��
}
