#include <dxlib.h>
#include "Shape.h"
#include "Circle.h"
#include "../_debug/_DebugConOut.h"

Shape::Shape(Vector2Flt& pos, Vector2Flt& size) : pos_(pos), size_(size)
{
	GetDrawScreenSize(&scrSize_.x, &scrSize_.y);			// �X�N���[���T�C�Y�擾
}

Shape::~Shape()
{
}

void Shape::Draw(void)
{
	DrawBox((int)pos_.x, (int)pos_.y, (int)pos_.x + (int)size_.x, (int)pos_.y + (int)size_.y, 0x00ffff, true);
}

void Shape::UpdateMoveDir(float up, float down, float left, float right)
{
	// ���ˏ���
	if (right >= scrSize_.x)
	{
		// �E�ǂɓ�������
		moveLR_ = MoveLR::Left;
		hitWall_ = WallDir::Left;
	}
	else if (left <= 0)
	{
		// ���ǂɓ�������
		moveLR_ = MoveLR::Right;
		hitWall_ = WallDir::Right;
	}
	else
	{
		// �ǂɓ������ĂȂ����
		hitWall_ = WallDir::Max;
	}
	if (down >= scrSize_.y)
	{
		// ���ǂɓ�������
		moveUD_ = MoveUD::Up;
		hitWall_ = WallDir::Up;
	}
	else if (up <= 0)
	{
		// ��ǂɓ�������
		moveUD_ = MoveUD::Down;
		hitWall_ = WallDir::Down;
	}
	else
	{
		// �ǂɓ������ĂȂ����
		hitWall_ = WallDir::Max;
	}
}

bool Shape::IsHitCircleAndSquare(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize)
{
	bool rtnFlag = true;
	if (sPos.x - cSize.x >= cPos.x ||
		sPos.x + sSize.x + cSize.x <= cPos.x ||
		sPos.y - cSize.y >= cPos.y ||
		sPos.y + sSize.y + cSize.y <= cPos.y)
	{
		rtnFlag = false;//��`�ɉ~�̔��a���𑫂����͈�
	}
	if (sPos.x >= cPos.x &&
		sPos.y >= cPos.y &&
		!((sPos.x - cPos.x) * (sPos.x - cPos.x) + (sPos.y + cPos.y) * (sPos.y + cPos.y) <= cSize.x * cSize.x))
	{
		rtnFlag = false;//����̓����蔻��
	}
	if (sPos.x + sSize.x <= cPos.x &&
		sPos.y >= cPos.y &&
		!((sPos.x + sSize.x - cPos.x) * (sPos.x + sSize.x - cPos.x) + (sPos.y + cPos.y) * (sPos.y + cPos.y) <= cSize.x * cSize.x))
	{
		rtnFlag = false;//�E��̓����蔻��
	}
	if (sPos.x >= cPos.x &&
		sPos.y + sSize.y <= cPos.y &&
		!((sPos.x - cPos.x) * (sPos.x - cPos.x) + (sPos.y + sSize.y + cPos.y) * (sPos.y + sSize.y + cPos.y) <= cSize.x * cSize.x))
	{
		rtnFlag = false;//�����̓����蔻��
	}
	if (sPos.x + sSize.x <= cPos.x &&
		sPos.y + sSize.y <= cPos.y &&
		!((sPos.x + sSize.x - cPos.x) * (sPos.x + sSize.x - cPos.x) + (sPos.y + sSize.y + cPos.y) * (sPos.y + sSize.y + cPos.y) <= cSize.x * cSize.x))
	{
		rtnFlag = false;//�E���̓����蔻��
	}
	return rtnFlag;
}

bool Shape::IsHitCircleAndTriangle(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize)
{
	auto diff = tPos - cPos;
	auto totalRad = tSize + cSize;
	if (totalRad.x * totalRad.x >= diff.x * diff.x + diff.y * diff.y)
	{
		return true;
	}
	return false;
}

bool Shape::IsHitCircleAndTree(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize)
{
	tSize.y = (cSize.y + 15) * 2;
	return IsHitCircleAndSquare(cPos, cSize, tPos, tSize);
}

bool Shape::IsHitCircleAndMicky(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt mPos, Vector2Flt mSize)
{
	mSize *= Vector2Flt{ 1.5,1.5 };
	return IsHitCircleAndTriangle(cPos, cSize, mPos, mSize);
}

bool Shape::IsHitCircleAndStar(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize)
{
	sPos.y += sSize.y;
	return IsHitCircleAndTriangle(cPos, cSize, sPos, sSize);
}

bool Shape::IsHitSquareAndTriangle(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize)
{
	return IsHitCircleAndSquare(tPos, tSize, sPos, sSize);
}

bool Shape::IsHitSquareAndTree(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize)
{
	if (sPos.x <= tPos.x + tSize.x * 2 / 3 &&
		sPos.x + sSize.x >= tPos.x - tSize.x / 3 &&
		sPos.y <= tPos.y + (tSize.y + 15) * 2 &&
		sPos.y + sSize.y >= tPos.y)
	{
		return true;
	}
	return false;
}

bool Shape::IsHitSquareAndMicky(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt mPos, Vector2Flt mSize)
{
	mSize *= Vector2Flt{ 1.5,1.5 };
	return IsHitCircleAndSquare(mPos, mSize, sPos, sSize);
}

bool Shape::IsHitSquareAndStar(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt stPos, Vector2Flt stSize)
{
	stPos.y += stSize.y;
	return IsHitCircleAndSquare(stPos, stSize, sPos, sSize);
}

bool Shape::IsHitTriangleAndTree(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt treePos, Vector2Flt treeSize)
{
	treeSize.y = (tSize.y + 15) * 2;
	return IsHitCircleAndSquare(treePos, treeSize, tPos, tSize);
}

bool Shape::IsHitTriangleAndMicky(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt mPos, Vector2Flt mSize)
{
	mSize *= Vector2Flt{ 1.5,1.5 };
	return IsHitCircleAndTriangle(mPos, mSize, tPos, tSize);
}

bool Shape::IsHitTriangleAndStar(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt sPos, Vector2Flt sSize)
{
	sPos.y += sSize.y;
	return IsHitCircleAndTriangle(sPos, sSize, tPos, tSize);
}
