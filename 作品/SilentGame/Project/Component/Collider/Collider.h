#pragma once
#include <functional>
#include <type_traits>
#include "../Component.h"

class Obj;
class CircleCollider;
class FanCollider;
class Collider;
using GetHitFunc = std::function<void(Collider&)>;


enum class CheckType : int
{
	Non = 0,			// ����������Ȃ�
	EnemyBody = 1 << 0,		// �G�̖{�̂̓����蔻��
	PlayerBody = 1 << 1,		// �v���C���[�̖{�̂̓����蔻��
	Box = 1 << 2,			// box
	Alert = 1 << 3,			// �A���[�g
	TrapAlarm = 1 << 4,		// �g���b�v
	SnareFloor = 1 << 5,	// �������鏰
	LandMine = 1 << 6,		// �n��
	Alcohol = 1 << 7,		// �A���R�[��
	Decoy = 1 << 8,		// �f�R�C
	Item = 1 << 9,			// �A�C�e��
	EnemyFan = 1 << 10,	// �G�̎��E
	EnemySound = 1 << 11,	// �G�̒���
	PlayerAttack = 1 << 12,// �G�̍U������
	Target = 1 << 13,
	Sound = 1 << 14,
	SecurityCam = 1 << 15,
	Door = 1 << 16,
	All = ~Non			// ���ׂĂƓ�����
};

// �����蔻��p�N���X�̊��N���X
class Collider :
	public Component
{
public:

	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="getHitFunc"> �����������̏�����function�� </param>
	/// <param name="owner"> ������ </param>
	Collider(CheckType targetType,const GetHitFunc& getHitFunc,Obj& owner);
	virtual ~Collider();

	/// <summary> �Ώۂ̃R���C�_�[�Ɠ������Ă��邩�𒲂ׂ鏈�� </summary>
	/// <param name="col"> �Ώۂ̃R���C�_�[ </param>
	virtual void HitCheck(Collider& col) = 0;

	virtual bool HitBoolCheck(Collider& col) = 0;

	/// <summary> �Ώۂ̃R���C�_�[�Ɠ������Ă��邩�𒲂ׂ鏈��(�����o��������) </summary>
	/// <param name="col">�Ώۂ̃R���C�_�[</param>
	virtual bool HitExCheck(Collider& col) = 0;

	/// <summary> �T�[�N���R���C�_�[�Ƃ̓����蔻��𒲂ׂ� </summary>
	/// <param name="col"> �T�[�N���R���C�_�[ </param>
	/// <returns></returns>
	virtual bool HitCheck(CircleCollider& col) = 0;

	/// <summary> �T�[�N���R���C�_�[�Ƃ̓����蔻��𒲂ׂ�(�����o��) </summary>
	/// <param name="col">�T�[�N���R���C�_�[</param>
	/// <returns></returns>
	virtual bool HitExCheck(CircleCollider& col) = 0;

	virtual bool HitCheck(FanCollider& col) = 0;

	/// <summary> ��������擾 </summary>
	/// <returns> ������(Obj�̎Q��) </returns>
	Obj& GetOwner() { return owner_; }

	/// <summary> �����������̔����������擾 </summary>
	/// <param name=""></param>
	/// <returns> std::function�̎Q�� </returns>
	GetHitFunc& GetFunc(void) { return getHitFunc_; }

	bool IsHit(void)const { return hitFlag_ && !oldHitFlag_; }
	
	bool IsHitStay(void) const { return hitFlag_ && oldHitFlag_; }

	bool IsHitRelese(void) const { return !hitFlag_ && oldHitFlag_; }

	void Hit(void) { hitFlag_ = true; }
	void HitFlagUpdate(void) { oldHitFlag_ = hitFlag_; }

	/// <summary> �^�[�Q�b�g�^�C�v�Ɋ܂܂�Ă��邩�H </summary>
	/// <param name="targetType"> ����̃^�C�v </param>
	/// <returns> �܂܂�Ă�����true�����łȂ����false </returns>
	const bool IsTargetType(CheckType targetType) const
	{ 
		auto r = (static_cast<std::underlying_type<CheckType>::type>(targetType) & targetType_);
		return  r > 0;
	}

	/// <summary> ���g�̎�ނ�Ԃ� </summary>
	/// <param name=""></param>
	/// <returns> ���g�̃^�C�v </returns>
	const CheckType GetMyCheckType(void) const { return myType_; }

	/// <summary> �����蔻�����鑊��̃^�C�v���Z�b�g���� </summary>
	/// <param name="targetType"> ��肽������̃^�C�v��int�ɂ���|�Ƃ��ŕ����w��ł��� </param>
	void SetTargetType(int targetType) { targetType_ = targetType; }
protected:
	bool hitFlag_;
	bool oldHitFlag_;

	/// <summary> �����������̏���������function </summary>
	GetHitFunc getHitFunc_;

	// ���g�̃^�C�v
	CheckType myType_;

	// �����蔻�����肽������
	std::underlying_type<CheckType>::type targetType_ = static_cast<std::underlying_type<CheckType>::type>(CheckType::All);
private:

	/// <summary> ID���擾���� </summary>
	/// <returns> ID </returns>
	ComponentID GetID() const override;
};

