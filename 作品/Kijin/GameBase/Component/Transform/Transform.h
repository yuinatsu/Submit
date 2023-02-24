#pragma once
#include <list>
#include <DxLib.h>
#include <fstream>
#include "../../Common/Math.h"
#include "../ComponentBase.h"



// �g�����X�t�H�[���̃N���X
class Transform :
	public ComponentBase
{
public:
	Transform();

	/// <summary>
	/// �q�I�u�W�F�N�g��ID��ǉ�
	/// </summary>
	/// <param name="id"> �q�I�u�W�F�N�g </param>
	void AddChild(const ObjectID& id);

	/// <summary>
	/// �q�I�u�W�F�N�g��ID�̍폜
	/// </summary>
	/// <param name="id"></param>
	void RemoveChild(const ObjectID& id);
	
	/// <summary>
	/// �e�I�u�W�F�N�g��ID���Z�b�g
	/// </summary>
	/// <param name="id"></param>
	void SetParent(const ObjectID& id)
	{
		parent_ = id;
	}

	/// <summary>
	/// �e�I�u�W�F�N�g��ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const ObjectID& GetParentID(void) const&
	{
		return parent_;
	}

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Sync(Transform& parent);
	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// �O�������̃x�N�g�����擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void) const;

	/// <summary>
	/// ������̃x�N�g�����擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetUp(void) const;


	/// <summary>
	/// �E�����̃x�N�g�����擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetRight(void) const;

	
	/// <summary>
	/// �g�����X�t�H�[�����w��x�N�g���֌X����
	/// </summary>
	/// <param name="vec"> �X��������̃x�N�g�� </param>
	void LookAt(const Vector3& vec);


	/// <summary>
	/// x,y,z�ŉ�]���Z�b�g����
	/// </summary>
	/// <param name="eulerRot"></param>
	void SetRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="eulerRot"></param>
	void AddRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// ��]���s����Z�b�g����
	/// </summary>
	/// <param name="matrix"> �s�� </param>
	void SetRotationMatrix(const MATRIX& matrix)
	{
		rot_ = matrix;
	}

	MATRIX& Rot(void)&
	{
		return rot_;
	}

	/// <summary>
	/// ��]��ǉ�����
	/// </summary>
	/// <param name="matrix"></param>
	void AddRotationMatrix(const MATRIX& matrix);

	/// <summary>
	/// ���[�J����]��x,y,z�ŃZ�b�g����
	/// </summary>
	/// <param name="eulerRot"></param>
	void SetLocalRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// ���[�J����]��x,y,z�Œǉ�����
	/// </summary>
	/// <param name="eulerRot"></param>
	void AddLocalRotFromEulerRot(const Vector3& eulerRot);

	void SetLocalRotaionMatrix(const MATRIX& matrix)
	{
		localRot_ = matrix;
	}

	/// <summary>
	/// ��]���̍s����擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ��]�̍s�� </returns>
	MATRIX GetRotationMatrix(void) const&
	{
		return MMult(MMult(localRot_,parentRot_), rot_);
	}

	const Vector3& GetPos(void) const&
	{
		return pos_;
	}

	Vector3& Pos(void)&
	{
		return pos_;
	}

	const Vector3& GetScale(void) const&
	{
		return scale_;
	}
	
	Vector3& Scale(void)&
	{
		return scale_;
	}

	Transform& operator=(const Transform& transform);

	

	/// <summary>
	/// �t�@�C������f�[�^��ǂݍ��ݓK������
	/// </summary>
	/// <param name="file"></param>
	void Read(std::ifstream& file);

	/// <summary>
	/// �t�@�C��������W�A��]�A�g�k�̃f�[�^���擾����
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static std::tuple<Vector3, Vector3, Vector3> Load(std::ifstream& file);

	SetID(ComponentID::Transform, ComponentID::Transform)
		ComponentID GetID(void) const override
	{
		return id_;
	}
private:

	// �q�I�u�W�F�N�g�̃��X�g
	std::list<ObjectID> children_;

	// �e�I�u�W�F�N�g
	ObjectID parent_;
	// �s��
	MATRIX rot_;

	// local���W
	MATRIX localRot_;

	// �e�̉�]
	MATRIX parentRot_;

	// ���W
	Vector3 pos_;

	// �g��k��
	Vector3 scale_{ 1.0f,1.0f,1.0f };

	// �e����̃I�t�Z�b�g�ʒu
	Vector3 offset_{ zeroVector3<float> };
};

