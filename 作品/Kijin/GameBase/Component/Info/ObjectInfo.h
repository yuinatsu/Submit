#pragma once
#include "../ComponentBase.h"
#include "ObjectAttribute.h"

// �I�u�W�F�N�g�̏���ێ�����N���X
class ObjectInfo :
    public ComponentBase
{
public:
	ObjectInfo();
	void Begin(ObjectManager& objectManager) final;

	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// �I�u�W�F�N�g���L�����𒲂ׂ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsActive(void) const;

	/// <summary>
	/// �I�u�W�F�N�g��j������
	/// </summary>
	/// <param name=""></param>
	void Destory(void)
	{
		isActive_ = false;
	}

	/// <summary>
	/// �t�@�N�g���[���g���č폜�������s����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool UseFactory(void)
	{
		return useFactory_;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}

	/// <summary>
	/// �t�@�N�g���[���g���폜�������s�����̃t���O���Z�b�g����
	/// </summary>
	/// <param name="flag"> �t���O </param>
	void SetUseFactory(const bool flag)
	{
		useFactory_ = flag;
	}

	/// <summary>
	/// �I�u�W�F�N�g�̑������Z�b�g����
	/// </summary>
	/// <param name="atr"> �Z�b�g����I�u�W�F�N�g�̑��� </param>
	void SetAttribute(const ObjectAttribute atr)
	{
		attribute_ = atr;
	}

	/// <summary>
	/// �I�u�W�F�N�g�̑������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns> �I�u�W�F�N�g�̑��� </returns>
	const ObjectAttribute GetAttribute(void) const
	{
		return attribute_;
	}

	SetID(ComponentID::Info, ComponentID::Info)
private:

	// active���̃t���O
	bool isActive_ = false;

	// factory���g�����폜���s����
	bool useFactory_ = false;

	// �I�u�W�F�N�g�̑���(�G���v���C���[�̂�����ȊO��)
	ObjectAttribute attribute_;
};

