#include "TerrainCollider.h"
#include "../../Application.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/ResourceMng.h"

TerrainCollider::TerrainCollider()
{
}

void TerrainCollider::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	
}

void TerrainCollider::Begin(ObjectManager& objectManager)
{
	render_ = objectManager.GetComponent<ModelRender>(ownerId_);
	//MV1SetRotationXYZ(*handel_, VGet(0.0f, Deg2Rad(180.0f), 0.0f));
	MV1SetupCollInfo(*handel_,-1,64,16,64,-1);
	MV1SetupCollInfo(*ground_);

}

void TerrainCollider::End(ObjectManager& objectManager)
{
	Collider::End(objectManager);
	
	MV1TerminateCollInfo(*handel_);
	MV1TerminateCollInfo(*ground_);
}

const int TerrainCollider::GetHandle(void) const
{
	return *handel_;
}

void TerrainCollider::Load(const std::filesystem::path& path)
{
	lpResourceMng.LoadModel(handel_, path);
}

void TerrainCollider::LoadGround(const std::filesystem::path& path)
{
	lpResourceMng.LoadModel(ground_, path);
}

#ifdef _DEBUG
void TerrainCollider::DrawDebug(void)
{
	if (IsActive())
	{
		MV1DrawModelDebug(*handel_, 0xaaffaa, TRUE, 10.0f, FALSE, FALSE);
	}
}
#endif

void TerrainCollider::Check(Collider& col, ObjectManager& objectManager)
{
	if (col.IsActive() && IsActive())
	{
		if (col.Check(*this, objectManager))
		{
			// 当たった時の処理
			Hit(col, objectManager);
			col.Hit(*this, objectManager);
		}
	}
}

bool TerrainCollider::Check(CapsuleCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col,*this);
}

bool TerrainCollider::Check(SphereCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col,*this);
}

bool TerrainCollider::Check(MeshCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col,*this);
}

bool TerrainCollider::Check(CharactorCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col,*this, objectManager);
}

bool TerrainCollider::Check(TerrainCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(*this,col);
}
