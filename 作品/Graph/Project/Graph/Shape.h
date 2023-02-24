#pragma once
#include <memory>
#include <vector>
#include "Vector2.h"

// �}�`���
enum class ShapeType
{
	Square,
	Circle,
	Triangle,
	Star,
	Tree,
	Micky,
	Max
};

// �c�ړ�����
enum class MoveUD
{
	Up,
	Down,
	Max
};

// ���ړ�����
enum class MoveLR
{
	Left,
	Right,
	Max
};

// �ǂ̈ʒu
enum class WallDir
{
	Up,
	Down,
	Left,
	Right,
	Max
};

class Shape;
using ShapeVec = std::vector<std::unique_ptr<Shape>>;

class Shape
{
public:
	Shape(Vector2Flt& pos, Vector2Flt& size);                  // �R���X�g���N�^(���W, �T�C�Y)
	~Shape();
	virtual void Update(float delta, ShapeVec& shapeVec) = 0;		// �X�V(deltaTime, ShapeVec)
	virtual void Draw(void) = 0;									// �`��
	virtual void Draw(float num) = 0;								// �`��(�g��T�C�Y)
	virtual void Draw(float num, Vector2Flt offset) {};				// �`��(�g��T�C�Y, ���S���W)
	virtual ShapeType GetType() = 0;								// �}�`��ނ��擾
	virtual Vector2Flt GetPos() = 0;								// ���W�擾
	virtual Vector2Flt GetSize() = 0;								// �T�C�Y�擾
	virtual int GetColor() = 0;										// �}�`�F�擾
private:
protected:
	void UpdateMoveDir(float up, float down, float left, float right);												// �ړ������`�F�b�N
	// �����蔻��
	bool IsHitCircleAndSquare(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize);				// �~*��`
	bool IsHitCircleAndTriangle(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize);				// �~*�O�p
	bool IsHitCircleAndTree(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize);					// �~*�c���[
	bool IsHitCircleAndMicky(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt mPos, Vector2Flt mSize);					// �~*�~�b�L�[
	bool IsHitCircleAndStar(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize);					// �~*��
	bool IsHitSquareAndTriangle(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize);				// ��`*�O�p
	bool IsHitSquareAndTree(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize);					// ��`*�c���[
	bool IsHitSquareAndMicky(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt mPos, Vector2Flt mSize);					// ��`*�~�b�L�[
	bool IsHitSquareAndStar(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt stPos, Vector2Flt stSize);				// ��`*��
	bool IsHitTriangleAndTree(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt treePos, Vector2Flt treeSize);			// �O�p*�c���[
	bool IsHitTriangleAndMicky(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt mPos, Vector2Flt mSize);				// �O�p*�~�b�L�[
	bool IsHitTriangleAndStar(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt sPos, Vector2Flt sSize);				// �O�p*��

	Vector2Flt pos_;		// ���W
	Vector2Flt size_;		// �T�C�Y

	Vector2 scrSize_;		// �X�N���[���T�C�Y

	MoveLR moveLR_;			// ���E�ړ�����
	MoveUD moveUD_;			// �㉺�ړ�����

	WallDir hitWall_;		// �ǔ���

	int color_;				// �F
	int speed_;				// �ړ���
};
