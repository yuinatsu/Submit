#include "StageFactory.h"
#include <DxLib.h>
#include <fstream>
#include "../Common/ResourceMng.h"
#include "../Application.h"
#include "../Object/ObjectManager.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Render/ModelRender.h"
#include "../Component/Collider/TerrainCollider.h"
#include "PlayerFactory.h"
#include "EnemyFactory.h"
#include "../Application.h"
#include "../Common/ThreadPool.h"
#include "../Common/ObjectData.h"
#include "../Common/Utility.h"
#include "../Factory/SkyFactory.h"
#include "../Component/Behavior/StageBehavior.h"
#include "../Common/Debug.h"



StageFactory::StageFactory(const std::filesystem::path& path, ObjectManager& objectManager) :
	Factory{objectManager}
{
	isLoaded_.store(false);
	path_ = path;
	AppThread.Add(std::bind(&StageFactory::Load, this));
	//Load();
}

void StageFactory::Load(void)
{
	funcMap_.emplace(FactoryID::Sky, &StageFactory::CreateSkyDome);
	funcMap_.emplace(FactoryID::Player, &StageFactory::CreatePlayer);
	funcMap_.emplace(FactoryID::Enemy, &StageFactory::CreateEnemy);

	id_ = objectManager_.MakeObjectID();
	objectManager_.AddComponent(std::make_unique<ObjectInfo>(), id_);
	objectManager_.AddComponent(std::make_unique<Transform>(), id_);
	objectManager_.AddComponent(std::make_unique<ModelRender>(), id_);
	objectManager_.AddComponent(std::make_unique<TerrainCollider>(), id_);
	objectManager_.AddComponent(std::make_unique<StageBehavior>(), id_);
	auto col = objectManager_.GetComponent<TerrainCollider>(id_);
	col->Load("Resource/resource/StageCol.mv1");
	/*auto id = objectManager_.MakeObjectID();
	objectManager_.AddComponent(std::make_unique<ObjectInfo>(), id);
	objectManager_.AddComponent(std::make_unique<Transform>(), id);
	objectManager_.AddComponent(std::make_unique<TerrainCollider>(), id);
	auto col2 = objectManager_.GetComponent<TerrainCollider>(id);
	col2->Load("Resource/resource/col.mv1");*/

	Factory::Load(path_.string());

	objectManager_.GetComponent<StageBehavior>(id_)->SetEnemyCount(enemyCount_);

	isLoaded_.store(true);
}

const bool StageFactory::IsLoaded(void)
{
	return isLoaded_.load();
}

void StageFactory::CreateSkyDome(std::ifstream& file)
{
	const auto& [pos, rot, scale] = Transform::Load(file);
	auto name = Utility::ReadString(file);
	 objectManager_.AddFactory(std::make_unique<SkyFactory>(name,objectManager_));
	 objectManager_.CreateFromFactory(FactoryID::Sky, id_, pos);
}

void StageFactory::CreatePlayer(std::ifstream& file)
{
	// ç¿ïWÇ∆Ç©
	const auto& [pos, rot, scale] = Transform::Load(file);

	auto name = Utility::ReadString(file);

	objectManager_.AddFactory(std::make_unique<PlayerFactory>(name, objectManager_));
	objectManager_.CreateFromFactory(FactoryID::Player, id_, pos);
}

void StageFactory::CreateEnemy(std::ifstream& file)
{
	const auto& [pos, rot, scale] = Transform::Load(file);
	auto name = Utility::ReadString(file);
	if (!objectManager_.HaveFactory(FactoryID::Enemy))
	{
		objectManager_.AddFactory(std::make_unique<EnemyFactory>(id_,name, objectManager_));
	}
	objectManager_.CreateFromFactory(FactoryID::Enemy, id_, pos);
	enemyCount_++;
}

void StageFactory::LoadTrans(std::ifstream& file, std::uint32_t size)
{
	auto transform = objectManager_.GetComponent<Transform>(id_);
	transform.Get()->Read(file);
}


void StageFactory::LoadModel(std::ifstream& file, std::uint32_t size)
{
	auto modelRender = objectManager_.GetComponent<Render>(id_);
	modelRender.Get()->Load(ModelRender::Load(file));
}

void StageFactory::LoadSpawn(std::ifstream& file, std::uint32_t size)
{
	FactoryID fID;
	file.read(reinterpret_cast<char*>(&fID), sizeof(fID));
	if (funcMap_.contains(fID))
	{
		(this->*funcMap_.at(fID))(file);
	}
}
