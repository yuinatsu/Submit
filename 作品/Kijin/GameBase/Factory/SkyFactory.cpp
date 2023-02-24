#include "SkyFactory.h"
#include "../Common/ObjectData.h"
#include "../Object/ObjectManager.h"
#include "../Component/Render/NonLightingRender.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Common/Debug.h"

SkyFactory::SkyFactory(const std::string& filePath, ObjectManager& objectManager) :
	Factory{objectManager}
{
	Factory::Load(filePath);
	
}

ObjectID SkyFactory::Create(ObjectID ownerID, const Vector3& pos, const Vector3& rot)
{
	auto id = objectManager_.MakeObjectID();
	 objectManager_.AddComponent(std::make_unique<ObjectInfo>(), id);
	 objectManager_.AddComponent(std::make_unique<Transform>(), id);
	 objectManager_.AddComponent(std::make_unique<NonLightingRender>(), id);

	 auto render = objectManager_.GetComponent<Render>(id);
	 render->Load(modelPath_);
	 auto transform = objectManager_.GetComponent<Transform>(id);
	 transform->Pos() = pos;
	 transform->Scale() = scale_;
	 return id;
}

void SkyFactory::LoadTrans(std::ifstream& file, std::uint32_t size)
{
	auto [pos, rot, scale] = Transform::Load(file);
	scale_ = scale;
}

void SkyFactory::LoadModel(std::ifstream& file, std::uint32_t size)
{
	modelPath_ = ModelRender::Load(file);
}


