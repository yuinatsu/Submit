#pragma once
#include <DxLib.h>
#include <memory>
#include <unordered_map>
#include <forward_list>
#include <map>
#include <mutex>
#include "../Component/ComponentHandle.h"
#include "../Factory/FactoryID.h"
#include "ObjectID.h"
#include "../Component/ComponentID.h"
#include "../Common/Vector3.h"
#include "../Component/Info/ObjectAttribute.h"

class BaseScene;
class ObjectManager;
class ObjectID;
class Factory;
class ComponentPool;
class Controller;

class ThreadPool;

// �I�u�W�F�N�g���Ǘ�����N���X
class ObjectManager
{
	using ComponentUptr = std::unique_ptr<ComponentBase>;
	using ComponentMap = std::unordered_map<size_t, ComponentUptr >;
public:
	ObjectManager(size_t objectMax);
	~ObjectManager();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="scene"> �V�[�� </param>
	void Update(float delta, Controller& controller, BaseScene& scene);

	/// <summary>
	/// �V���h�E�}�b�v���l�����ĕ`�悷��
	/// </summary>
	/// <param name="shadowMap"></param>
	/// <param name="buff"></param>
	void ShadowDraw(int shadowMap = -1,int buff = -1);

	/// <summary>
	/// �[�x�e�N�X�`�����쐬
	/// </summary>
	/// <param name="ps">�[�x�쐬�p�̃s�N�Z���V�F�[�_</param>
	void SetupDepthTex(int ps = -1,int buffer = -1);

	/// <summary>
	/// �w��I�u�W�F�N�gID�̃R���|�[�l���g���擾
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	template<CompC T>
	ComponentHandle<T> GetComponent(const ObjectID& id);

	/// <summary>
	/// �I�u�W�F�N�g���A�N�e�B�u����Ԃ�
	/// </summary>
	/// <param name="id"> �I�u�W�F�N�gID </param>
	/// <returns></returns>
	bool IsActive(const ObjectID& id);

	/// <summary>
	/// ID�𐶐�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID MakeObjectID(void);

	/// <summary>
	/// �R���|�[�l���g�̒ǉ�
	/// </summary>
	/// <param name="component"> �ǉ�����R���|�[�l���g </param>
	/// <param name="id"> ID </param>
	/// <returns></returns>
	template<CompC T>
	bool AddComponent(std::unique_ptr<T> component, ObjectID& id);

	/// <summary>
	/// �R���|�[�l���g�̍폜
	/// </summary>
	/// <param name="id"> �폜����R���|�[�l���g�����I�u�W�F�N�gID </param>
	/// <returns> �폜�����R���|�[�l���g </returns>
	template<CompC T>
	std::unique_ptr<T> RemoveComponent(ObjectID& id);

	/// <summary>
	/// �w�肵���R���|�[�l���g�������Ă��邩�H
	/// </summary>
	/// <param name="id"> ���ׂ�I�u�W�F�N�g��ID </param>
	/// <returns></returns>
	template<CompC T>
	bool HaveComponent(const ObjectID& id);

	/// <summary>
	/// ���ׂẴR���|�[�l���g���폜����
	/// </summary>
	/// <param name="id"> �Ώۂ̃I�u�W�F�N�g��ID </param>
	void RemoveAllComponent(const ObjectID& id);

	/// <summary>
	/// �J�n����
	/// </summary>
	/// <param name=""></param>
	void Begin(void);

	/// <summary>
	/// �w���ID�̃I�u�W�F�N�g���J�n��������
	/// </summary>
	/// <param name="id"></param>
	void Begin(ObjectID& id);

	/// <summary>
	/// �w��̃I�u�W�F�N�g�̏I������������
	/// </summary>
	/// <param name="id"></param>
	void End(ObjectID& id);

	/// <summary>
	/// �����p�N���X�̓o�^
	/// </summary>
	/// <param name="factory"></param>
	void AddFactory(std::unique_ptr<Factory>&& factory);

	/// <summary>
	/// �����p�N���X�̎擾
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	Factory& GetFactory(const FactoryID id);

	/// <summary>
	/// �t�@�N�g���[ID�̃I�u�W�F�N�g���w����W�ɐ���
	/// </summary>
	/// <param name="id"> �g�p����factory��ID </param>
	/// <param name="pos"> ����������W </param>
	ObjectID CreateFromFactory(const FactoryID id, const ObjectID& objID, const Vector3 &pos, const Vector3& rot = { 0.0f, 0.0f, 0.0f});

	/// <summary>
	/// �����p�N���X�����݂��邩
	/// </summary>
	/// <param name="id"> �����p�N���X��ID </param>
	/// <returns></returns>
	bool HaveFactory(const FactoryID id);

	/// <summary>
	/// ���[�h�������Ă邩�H
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsLoaded(void);

	/// <summary>
	/// �v���C���[��ID
	/// </summary>
	/// <param name="id"></param>
	void SetPlayerID(ObjectID id)
	{
		playerID_ = id;
	}

	/// <summary>
	/// �v���C���[ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID& GetPlayerID(void)
	{
		return playerID_;
	}

	/// <summary>
	/// �v���C���[�U����ID
	/// </summary>
	/// <param name="id"></param>
	void SetPlayerAttackID(ObjectID id)
	{
		playerAttackID_ = id;
	}

	/// <summary>
	/// �v���C���[�U��ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID& GetPlayerAttackID(void)
	{
		return playerAttackID_;
	}

	/// <summary>
	/// �G�l�~�[��ID
	/// </summary>
	/// <param name="id"></param>
	void SetEnemyID(ObjectID id)
	{
		enemyID_ = id;
	}

	/// <summary>
	/// �G�l�~�[ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID& GetEnemyID(void)
	{
		return enemyID_;
	}

	ComponentPool& GetPool(void);

	/// <summary>
	/// �q�b�g�X�g�b�v���̉��o���J�n����
	/// </summary>
	/// <param name="stopTime"></param>
	void StartHitStop(const float stopTime);

	/// <summary>
	/// �A�g���r���[�g�Ɉ�v����I�u�W�F�N�g��T��
	/// </summary>
	/// <param name="atr"> �T�������I�u�W�F�N�g�̃A�g���r���[�g </param>
	std::pair<bool, ObjectID> Find(ObjectAttribute atr);


	/// <summary>
	/// �J��������I�u�W�F�N�g��ID���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetCameraID(ObjectID& id)
	{
		cameraID_ = id;
	}

	/// <summary>
	/// �J��������I�u�W�F�N�g��ID���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectID& GetCameraID(void)&
	{
		return cameraID_;
	}

	ObjectID& GetStageID(void)&
	{
		return stageID_;
	}

	void SetStageID(ObjectID& id)
	{
		stageID_ = id;
	}
private:

	// �v���C���[��ID
	ObjectID playerID_;

	// �v���C���[�U����ID
	ObjectID playerAttackID_;

	// �G�l�~�[��ID
	ObjectID enemyID_;

	// �J��������p
	ObjectID cameraID_;

	// �R���|�[�l���g���}�b�v�Ŏ���
	std::map<ComponentID, ComponentMap > componentMap_;

	// �����p�N���X
	std::map<FactoryID, std::unique_ptr<Factory>> factoryMap_;

	// �v�[���p�N���X
	std::unique_ptr<ComponentPool> pool_;

	// �q�b�g�X�g�b�v�̉��o�����Ă��邩
	bool isHitStop_;

	// �q�b�g�X�g�b�v���o�̎���
	float hitStopTime_;

	// �~���[�e�b�N�X
	std::mutex mutex_;

	// �X�e�[�W��ID
	ObjectID stageID_;
};

template<CompC T>
inline ComponentHandle<T> ObjectManager::GetComponent(const ObjectID& id)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (componentMap_.contains(T::baseID_))
	{
		if (componentMap_[T::baseID_].contains(*id))
		{
			auto r = (static_cast<unsigned int>(T::id_) & static_cast<unsigned int>(componentMap_[T::baseID_][*id]->GetID()));
			if (r == static_cast<unsigned int>(T::id_))
			{
				return ComponentHandle<T>{ id,static_cast<T*>(componentMap_[T::baseID_][*id].get()), this};
			}
		}
	}
	return ComponentHandle<T>{id,nullptr,this};
}

template<CompC T>
inline bool ObjectManager::AddComponent(std::unique_ptr<T> component, ObjectID& id)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (componentMap_[T::baseID_].contains(*id))
	{
		return false;
	}
	component->SetOwnerID(id);
	componentMap_[T::baseID_][*id] = std::move(component);
	return true;
}


template<CompC T>
inline std::unique_ptr<T> ObjectManager::RemoveComponent(ObjectID& id)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (componentMap_.contains(T::baseID_))
	{
		if (componentMap_[T::baseID_].contains(*id))
		{
			auto comp = std::move(componentMap_[T::baseID_][*id]);
			componentMap_[T::baseID_].erase(*id);
			auto r = static_cast<unsigned int>(T::id_) & static_cast<unsigned int>(comp->GetID());
			if (r == static_cast<unsigned int>(T::id_))
			{
				auto ptr = comp.get();
				comp.release();
				return std::unique_ptr<T>{static_cast<T*>(ptr)};
			}
		}
	}
	return std::unique_ptr<T>{nullptr};
}

template<CompC T>
inline bool ObjectManager::HaveComponent(const ObjectID& id)
{
	return componentMap_.contains(T::baseID_);
}

