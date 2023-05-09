#pragma once
#include <list>
#include <DxLib.h>
#include <fstream>
#include "../../Common/Math.h"
#include "../../Common/Quaternion.h"
#include "../ComponentBase.h"



// �g�����X�t�H�[���̃N���X
class Transform :
	public ComponentBase
{
public:
	Transform();
	
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;


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
	/// ��]��ǉ�����(�N�H�[�^�j�I��)
	/// </summary>
	/// <param name="addRotation"> �ǉ������](�N�H�[�^�j�I��) </param>
	/// <returns></returns>
	void AddRotation(const Quaternion& addRotation) &
	{
		rot_ *= addRotation;
	}

	/// <summary>
	/// ��]���Z�b�g����(�N�H�[�^�j�I��)
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const Quaternion& rotation) &
	{
		rot_ = rotation;
	}

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Quaternion GetRotation(void) const;


	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetPos(void) const&
	{
		return pos_;
	}

	/// <summary>
	/// ���W�̎Q�Ƃ̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Vector3& Pos(void)&
	{
		return pos_;
	}

	/// <summary>
	/// �X�P�[�����O�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetScale(void) const&
	{
		return scale_;
	}
	
	/// <summary>
	/// �X�P�[�����O�̎Q�Ƃ̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
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

	Quaternion rot_;

	// ���W
	Vector3 pos_;

	// �g��k��
	Vector3 scale_{ 1.0f,1.0f,1.0f };

	// �e����̃I�t�Z�b�g�ʒu
	Vector3 offset_{ zeroVector3<float> };
};

