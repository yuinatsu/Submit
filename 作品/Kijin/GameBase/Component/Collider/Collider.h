#pragma once
#include "../ComponentBase.h"
#include <functional>
#include "../ComponentHandle.h"
#include "ColliderType.h"
#include "../Transform/Transform.h"

class CapsuleCollider;
class SphereCollider;
class MeshCollider;
class CharactorCollider;
class TerrainCollider;


// �����蔻��p�N���X�̂��ƂɂȂ�N���X
class Collider :
	public ComponentBase
{
public:
	virtual ~Collider();
	void Begin(ObjectManager& objectManager) override;

	void End(ObjectManager& objectManager) override;
	
	// �`�F�b�N�p�֐�
	virtual void Check(Collider& col, ObjectManager& objectManager) = 0;
	virtual bool Check(CapsuleCollider& col, ObjectManager& objectManager) = 0;
	virtual bool Check(SphereCollider& col, ObjectManager& objectManager) = 0;
	virtual bool Check(MeshCollider& col, ObjectManager& objectManager) = 0;
	virtual bool Check(CharactorCollider& col, ObjectManager& objectManager) = 0;
	virtual bool Check(TerrainCollider& col, ObjectManager& objectManager) = 0;

	/// <summary>
	/// �q�b�g���ɌĂԃN���X
	/// </summary>
	/// <param name="col"> ������������̃R���C�_�[ </param>
	void Hit(Collider& col, ObjectManager& objMng);

	/// <summary>
	/// �I�t�Z�b�g�̃Z�b�g
	/// </summary>
	/// <param name="offset_"></param>
	void SetOffset(const Vector3& offset)
	{
		offset_ = offset;
	}

	/// <summary>
	/// �I�t�Z�b�g�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetOffet(void) const&
	{
		return offset_;
	}

	/// <summary>
	/// �q�b�g���ɌĂяo�����function���Z�b�g����
	/// </summary>
	/// <param name="func"></param>
	void SetHitFunc(std::function<void(Collider&, ObjectManager&)>&& func)
	{
		hit_ = std::move(func);
	}

	/// <summary>
	/// �����蔻�肪�L����
	/// </summary>
	/// <param name=""></param>
	/// <returns> �L����true������false </returns>
	const bool IsActive(void) const
	{
		return isActive_;
	}

	/// <summary>
	/// �����蔻���L���ɂ��邩���Z�b�g����
	/// </summary>
	/// <param name="flag"></param>
	void SetActiveFlag(const bool flag)
	{
		isActive_ = flag;
	}

	/// <summary>
	/// �����o���������s�����H
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsBlock(void) const 
	{
		return isBlock_;
	}

	/// <summary>
	/// �����o���Ɋւ���t���O���Z�b�g����
	/// </summary>
	/// <param name="flag"></param>
	void SetBlockFlag(const bool flag) 
	{
		isBlock_ = flag;
	}

 #ifdef _DEBUG
	virtual void DrawDebug(void) = 0;
#endif

	ComponentID GetID(void) const override
	{
		return id_;
	}
	SetID(ComponentID::Collider, ComponentID::Collider)

protected:

	// �J�v�Z���Ƃ̔���p�֐�
	bool HitCheck(CapsuleCollider& a, CapsuleCollider& b);
	bool HitCheck(CapsuleCollider& a, SphereCollider& b);
	bool HitCheck(CapsuleCollider& a, MeshCollider& b);
	bool HitCheck(CapsuleCollider& a, CharactorCollider& b);
	bool HitCheck(CapsuleCollider& a, TerrainCollider& b);

	// ���̂Ƃ̔���p�֐�
	bool HitCheck(SphereCollider& a, SphereCollider& b);
	bool HitCheck(SphereCollider& a, MeshCollider& b);
	bool HitCheck(SphereCollider& a, CharactorCollider& b);
	bool HitCheck(SphereCollider& a, TerrainCollider& b, ObjectManager& objectmanager);

	// ���b�V���Ƃ̔���p�֐�
	bool HitCheck(MeshCollider& a, MeshCollider& b);
	bool HitCheck(MeshCollider& a, CharactorCollider& b);
	bool HitCheck(MeshCollider& a, TerrainCollider& b);

	// �L�����N�^�[�Ƃ̔���p�֐�
	bool HitCheck(CharactorCollider& a, CharactorCollider& b, ObjectManager& objectmanager);
	bool HitCheck(CharactorCollider& a, TerrainCollider& b, ObjectManager& objectmanager);

	// �n�`�Ƃ̔���p�֐�
	bool HitCheck(TerrainCollider& a, TerrainCollider& b);

	// �����傩��̃I�t�Z�b�g
	Vector3 offset_;

	// ������̃g�����X�t�H�[���N���X
	ComponentHandle<Transform> transform_;

	// ���肪�L�����H
	bool isActive_{true};

	// �����o�����L����?
	bool isBlock_{ false };
private:

	// �q�b�g���ɌĂяo��function
	std::function<void(Collider&, ObjectManager&)> hit_;
};

