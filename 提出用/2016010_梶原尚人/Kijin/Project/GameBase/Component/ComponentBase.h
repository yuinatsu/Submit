#pragma once
#include "ComponentID.h"
#include "../Object/ObjectID.h"

class BaseScene;
class ObjectManager;
class Controller;

#define SetID(a,b) static constexpr ComponentID id_{a}; static constexpr ComponentID baseID_{ b };

// �Q�[���I�u�W�F�N�g�̃R���|�[�l���g�̊��N���X
class ComponentBase
{
public:
	virtual ~ComponentBase();

	/// <summary>
	/// �I�u�W�F�N�g���L���ɂȂ������ɌĂ΂��
	/// </summary>
	/// <param name="id">  </param>
	virtual void Begin(ObjectManager& objectManager);

	/// <summary>
	/// �I�u�W�F�N�g�������ɂȂ������ɌĂ΂��
	/// </summary>
	/// <param name=""></param>
	virtual void End(ObjectManager& objectManager){}

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="scene"> ���݂̃V�[�� </param>
	/// <param name="objectManager"> �I�u�W�F�N�g�}�l�[�W���[ </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller){}

	/// <summary>
	/// ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual ComponentID GetID(void) const = 0;

	/// <summary>
	/// �������ID���Z�b�g
	/// </summary>
	/// <param name="id"> �������ID </param>
	void SetOwnerID(ObjectID& id) { ownerId_ = id; }

	/// <summary>
	/// �������ID���擾
	/// </summary>
	/// <param name="">  </param>
	/// <returns> �������ID </returns>
	ObjectID& GetOwnerID(void) { return ownerId_; }

	SetID(ComponentID::Base, ComponentID::Base)
protected:

	// �������ID
	ObjectID ownerId_;
};


