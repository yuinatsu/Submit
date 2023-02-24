#pragma once
#include <DxLib.h>
class Transform;

class Capsule
{

public :

	static constexpr int COLOR = 0xffffff;

	Capsule(Transform* parent);

	// �`��
	void Draw(void);

	// �eTransform����̑��Έʒu���Z�b�g
	void SetRelativePosTop(VECTOR pos);
	void SetRelativePosDown(VECTOR pos);

	VECTOR GetPosTop(void);
	VECTOR GetPosDown(void);
	VECTOR GetRelativePos(VECTOR pos);

	// ���a
	float GetRadius(void);
	void SetRadius(float radius);

	// ����
	float GetHeight(void);

	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void);

	// �R�s�[
	Capsule Copy(Transform* base);

	void SetTransformParent(Transform* parent);

private :

	Transform* mTransformParent;

	// �eTransform����̑��Έʒu(�㑤)
	VECTOR mRelativePosTop;

	// �eTransform����̑��Έʒu(����)
	VECTOR mRelativePosDown;

	// ���a
	float mRadius;

};

