#pragma once
#include "Shape.h"

class Square :
    public Shape
{
public:
	Square(Vector2Flt&& pos, Vector2Flt&& size);					// �R���X�g���N�^(���W, �T�C�Y)// �E�Ӓl�Q�ƁF&&
	~Square();
	void Update(float delta, ShapeVec& shapeVec) override;          // �X�V(deltaTime, ShapeVec)
	void Draw(void) override;                                       // �`��
	void Draw(float num) override;                                  // �`��(�C�ӂ̃T�C�Y�ύX��)
	void Draw(float num, Vector2Flt offset) override;				// �`��(�C�ӂ̃T�C�Y�ύX�A�}�`�̒��S���W)
	ShapeType GetType() override { return ShapeType::Square; };		// shape�̎�ނ��擾
	Vector2Flt GetPos() override { return pos_; };                  // ���W�擾
	Vector2Flt GetSize() override { return size_; };                // �T�C�Y�擾
	int GetColor() override { return color_; };                     // �}�`�F���擾
};

