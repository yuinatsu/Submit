#pragma once
#include "Collider.h"

class Collider;

// ‰~Œ`‚Ì“–‚½‚è”»’è
class CircleCollider :
	public Collider
{
public:
	/// <summary> ƒRƒ“ƒXƒgƒ‰ƒNƒ^ </summary>
	/// <param name="r"> ”¼Œa </param>
	/// <param name="gethitFunc"> “–‚½‚Á‚½‚Ìˆ— </param>
	/// <param name="obj"> ‚¿å </param>
	CircleCollider(float r, CheckType myType, const GetHitFunc& getHitFunc, Obj& obj);
	~CircleCollider();

	/// <summary> ”¼Œa‚ğæ“¾ </summary>
	/// <param name=""></param>
	/// <returns> ”¼Œa </returns>
	const float GetRadius(void)const;

	/// <summary> ”¼Œa‚Ìİ’è </summary>
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
	/// <summary> ”¼Œa </summary>
	float r_;
};

