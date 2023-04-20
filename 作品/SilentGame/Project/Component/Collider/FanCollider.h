#pragma once
#include "../../common/Math.h"
#include "Collider.h"
#include <memory>

class CircleCollider;
class TmxObj;


class FanCollider :
    public Collider
{
public:
	FanCollider(std::shared_ptr<TmxObj> tmxObj,float r, float angle, CheckType myType,const GetHitFunc& getHitFunc, Obj& obj);
	FanCollider(std::shared_ptr<TmxObj> tmxObj,const Math::Vector2& inv1, const Math::Vector2& inv2, CheckType myType,const GetHitFunc& getHitFunc, Obj& obj);
	void AddAngle(const float angle);

	// ���E�̃x�N�g������Ɋp�x��ς���
	void SetAngleFromVec(const Math::Vector2& inv1, const Math::Vector2& inv2);
	bool HitCheck(const Math::Vector2& pos);
	void Update(float delta) override;

	const Math::Vector2& GetV1Vec(void)const { return v1_; }
	const Math::Vector2& GetV2Vec(void)const { return v2_; }
	bool CheckCol(const Math::Vector2& startPos, const Math::Vector2& vec);
	bool CheckCol(void);
private:
#ifdef _DEBUG
	void DbgDraw(const Math::Vector2& offset) const override;
#endif

	void HitCheck(Collider& col) override;
	bool HitBoolCheck(Collider& col) override { return false; };
	bool HitExCheck(Collider& col) override;
	bool HitCheck(CircleCollider& col) override;
	bool HitExCheck(CircleCollider& col) override;
	bool HitCheck(FanCollider& col) override;

	

	Math::Vector2 v1_;					// ��`�̒[�x�N�g��
	Math::Vector2 v2_;					// ��`�̒[�x�N�g��
	float defLength_;
	std::shared_ptr<TmxObj> tmxObj_;
	//Math::Vector2 vv1_;				// ��`�̒[�x�N�g��(�p�x�����߂邽�߂�v1�������ꏊ)
	//Math::Vector2 vv2_;				// ��`�̒[�x�N�g��()
	//Math::Vector2 vf1_;				// �ǂɓ������Ă���Ƃ��̐�`�̒[�x�N�g��
	//Math::Vector2 vf2_;				// �ǂɓ������Ă���Ƃ��̐�`�̒[�x�N�g��
};

