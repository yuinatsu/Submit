#pragma once
#include "../ComponentBase.h"

class Collider;
class ObjectManager;


// �I�u�W�F�N�g�̓��������s����N���X
class Behavior :
	public ComponentBase
{
public:

	virtual ~Behavior();

	ComponentID GetID(void) const override
	{
		return id_;
	}
	SetID(ComponentID::BehaviorBase, ComponentID::BehaviorBase)

	/// <summary>
	/// �j��������ʓr�s�����߂̊֐�
	/// </summary>
	/// <param name="objManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	virtual void Destory(ObjectManager& objManager){}
};

