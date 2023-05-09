#include "ObjectManager.h"
#include <algorithm>
#include "../Component/ComponentPool.h"
#include "../Component/Render/Render.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Component/Behavior/Behavior.h"
#include "ObjectID.h"
#include "../Factory/Factory.h"
#include "../Component/Collider/Collider.h"
#include "../SceneManager.h"
#include "../Common/Debug.h"

#include "../Common/ThreadPool.h"

constexpr int compNum{4};
constexpr int maxSeed{ 8 };

bool ObjectManager::IsActive(const ObjectID& id)
{
	if (componentMap_.contains(ComponentID::Info))
	{
		if (componentMap_.at(ComponentID::Info).contains(*id))
		{
			return static_cast<ObjectInfo&>(*componentMap_.at(ComponentID::Info).at(*id)).IsActive();
		}
	}
	return false;
}

ObjectID ObjectManager::MakeObjectID(void)
{
	// ���݂̎��Ԃ�Seed�Ƃ��ăI�u�W�F�N�g��ID�𐶐�����
	ObjectID id{ std::chrono::system_clock::now().time_since_epoch().count() };
	if (!componentMap_[ComponentID::Info].contains(*id))
	{
		return id;
	}
	assert(false);
	return ObjectID{ std::chrono::system_clock::now().time_since_epoch().count() };
}



void ObjectManager::RemoveAllComponent(const ObjectID& id)
{
	auto key = *id;
	// ���ׂẴR���|�[�l���g���`�F�b�N����
	for (auto& obj : componentMap_)
	{
		if (obj.second.contains(key))
		{
			// id������ꍇ�폜
			obj.second.erase(key);
		}
	}
}

void ObjectManager::Begin(void)
{
	DebugLog("behavior�̐�", componentMap_[ComponentID::BehaviorBase].size());
	for (auto& comps : componentMap_)
	{
		for (auto& comp : comps.second)
		{
			comp.second->Begin(*this);
		}
	}
	DebugLog("behavior�̐�", componentMap_[ComponentID::BehaviorBase].size());
}

void ObjectManager::Begin(ObjectID& id)
{
	for (auto& comps : componentMap_)
	{
		if (comps.second.contains(*id))
		{
			comps.second[*id]->Begin(*this);
		}
	}
}

void ObjectManager::End(ObjectID& id)
{
	for (auto& c : componentMap_)
	{
		if (c.second.contains(*id))
		{
			c.second[*id]->End(*this);
		}
	}
}

void ObjectManager::AddFactory(std::unique_ptr<Factory>&& factory)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	factoryMap_.emplace(factory->GetID(), std::move(factory));
}

Factory& ObjectManager::GetFactory(const FactoryID id)
{
	return *factoryMap_[id];
}

ObjectID ObjectManager::CreateFromFactory(const FactoryID id,const ObjectID& objID, const Vector3& pos, const Vector3& rot)
{
	if (factoryMap_.contains(id))
	{
		return factoryMap_[id]->Create(objID, pos, rot);
	}
	return ObjectID{};
}



bool ObjectManager::HaveFactory(const FactoryID id)
{
	return factoryMap_.contains(id);
}

const bool ObjectManager::IsLoaded(void)
{
	for (auto& f : factoryMap_)
	{
		if (!f.second->IsLoaded())
		{
			return false;
		}
	}
	return true;
}

ComponentPool& ObjectManager::GetPool(void)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	return *pool_;
}

void ObjectManager::StartHitStop(const float stopTime)
{
	isHitStop_ = true;
	hitStopTime_ = stopTime;
}

std::pair<bool, ObjectID> ObjectManager::Find(ObjectAttribute atr)
{
	// �w��̑��������I�u�W�F�N�gID���擾���܂�
	auto itr = std::find_if(
		componentMap_[ComponentID::Info].begin(), 
		componentMap_[ComponentID::Info].end(),
		[atr](auto& pair) {
			return static_cast<ObjectInfo&>(*pair.second).GetAttribute() == atr;
		}
	);

	if (itr != componentMap_[ComponentID::Info].end())
	{
		return { true, itr->second->GetOwnerID() };
	}
	return { false, ObjectID{} };
}

ObjectManager::ObjectManager(size_t objectMax)
{
	isHitStop_ = false;
	hitStopTime_ = 0.0f;
	componentMap_[ComponentID::BehaviorBase].reserve(objectMax);
	componentMap_[ComponentID::RenderBase].reserve(objectMax);
	componentMap_[ComponentID::Info].reserve(objectMax);
	componentMap_[ComponentID::Transform].reserve(objectMax);
	componentMap_[ComponentID::Collider].reserve(objectMax);

	pool_ = std::make_unique<ComponentPool>();
	
}

ObjectManager::~ObjectManager()
{
	for (auto& comp : componentMap_)
	{
		for (auto& c : comp.second)
		{
			c.second->End(*this);
		}
	}
}

void ObjectManager::Update(float delta, Controller& controller, BaseScene& scene)
{
	// ���ׂĂ̕`��
	float d = delta;

	if (isHitStop_)
	{
		hitStopTime_ -= delta;
		d *= 0.1f;
		if (hitStopTime_ <= 0.0f)
		{
			isHitStop_ = false;
		}
	}
	

	// �r�w�C�r�A�̍X�V
	for (auto& b : componentMap_[ComponentID::BehaviorBase])
	{
		b.second->Update(scene, *this, d, controller);
	}

	// �g�����X�t�H�[���̍X�V
	for (auto& b : componentMap_[ComponentID::Transform])
	{
		b.second->Update(scene, *this, d, controller);
	}

	for (auto& b : componentMap_[ComponentID::Collider])
	{
		b.second->Update(scene, *this, d, controller);
	}

	// �����蔻��̏���
	if (!componentMap_.empty() && componentMap_[ComponentID::Collider].size() > 0)
	{
		auto start = componentMap_[ComponentID::Collider].begin();
		auto end = --componentMap_[ComponentID::Collider].end();
		for (auto b = start; b != end; ++b)
		{
			if (start == --componentMap_[ComponentID::Collider].end())
			{
				break;
			}

			for (auto s = ++start; s != componentMap_[ComponentID::Collider].end(); ++s)
			{
				static_cast<Collider&>(*(*b).second).Check(static_cast<Collider&>(*(*s).second), *this);
			}
		}
	}

	// �`��n�̍X�V
	for (auto& b : componentMap_[ComponentID::RenderBase])
	{
		b.second->Update(scene, *this, d, controller);
	}

	// �A�j���[�V�����̍X�V
	for (auto& b : componentMap_[ComponentID::Animator])
	{
		b.second->Update(scene, *this, d, controller);
	}

	// �I�u�W�F�N�g�̔j�����邩�̏���
	for (auto b = componentMap_[ComponentID::Info].begin(),e = componentMap_[ComponentID::Info].end(); b != e;)
	{
		if (!static_cast<ObjectInfo&>(*b->second).IsActive())
		{
			auto itr = b;

			bool isFactory = static_cast<ObjectInfo&>(*itr->second).UseFactory();

			// �j������̂�End���Ă�
			End(itr->second->GetOwnerID());
			++b;
			if (isFactory)
			{
				// �t�@�N�g���[���g�p���Ĕj���������s����
				static_cast<Behavior&>(*componentMap_[ComponentID::BehaviorBase][itr->first]).Destory(*this);
			}
			else
			{
				// �R���|�[�l���g�����ׂĂ��폜����
				RemoveAllComponent(itr->second->GetOwnerID());
			}
		}
		else
		{
			++b;
		}
		
	}
}



void ObjectManager::ShadowDraw(int shadowMap, int buff)
{
	for (auto& c : componentMap_.at(ComponentID::RenderBase))
	{
		static_cast<Render&>(*c.second).Draw(shadowMap, buff);
	}

#ifdef _DEBUG

	for (auto& c : componentMap_.at(ComponentID::Collider))
	{
		static_cast<Collider&>(*c.second).DrawDebug();
	}

#endif
}

void ObjectManager::SetupDepthTex(int ps, int buffer)
{
	for (auto& c : componentMap_.at(ComponentID::RenderBase))
	{
		static_cast<Render&>(*c.second).SetUpDepthTex(ps,buffer);
	}
}
