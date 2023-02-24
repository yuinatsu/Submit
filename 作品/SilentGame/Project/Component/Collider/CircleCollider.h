#pragma once
#include "Collider.h"

class Collider;

// �~�`�̓����蔻��
class CircleCollider :
	public Collider
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="r"> ���a </param>
	/// <param name="gethitFunc"> �����������̏��� </param>
	/// <param name="obj"> ������ </param>
	CircleCollider(float r, CheckType myType, const GetHitFunc& getHitFunc, Obj& obj);
	~CircleCollider();

	/// <summary> ���a���擾 </summary>
	/// <param name=""></param>
	/// <returns> ���a </returns>
	const float GetRadius(void)const;

	/// <summary> ���a�̐ݒ� </summary>
	/// <param name="r"></param>
	void SetRadius(float r);

	void HitCheck(Collider& col) override;
	bool HitBoolCheck(Collider& col) override;
	bool HitExCheck(Collider& col) override;
	bool HitCheck(CircleCollider& col) override;
	bool HitExCheck(CircleCollider& col) override;
	bool HitCheck(FanCollider& col) override;
private:

#ifdef _DEBUG

	void DbgDraw(const Math::Vector2& offset) const override;

#endif
	/// <summary> ���a </summary>
	float r_;
};

