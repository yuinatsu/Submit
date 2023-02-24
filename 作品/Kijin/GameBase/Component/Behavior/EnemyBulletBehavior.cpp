#include "EnemyBulletBehavior.h"
#include "../../Factory/EnemyBullet.h"
#include "../Info/ObjectInfo.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/Debug.h"
#include "EnemyBehavior.h"

constexpr float speed_{600.0f};

void EnemyBulletBehavior::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Scale() = Vector3{10.0f,10.0f,10.0f};
	collider_ = objectManager.GetComponent<SphereCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBulletBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	startPos_ = transform_->GetPos();
}

void EnemyBulletBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	transform_->Pos() += transform_->GetForward() * (speed_ * delta);
	auto info = objectManager.GetComponent<ObjectInfo>(ownerId_);
	//DebugLog(info->IsActive() ? "true" : "false");
	//DebugLog("x=", transform_->pos_.x, "y=", transform_->pos_.y,"z=", transform_->pos_.z);
	if ((transform_->GetPos() - startPos_).SqMagnitude() >= Square(5000.0f))
	{
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
	}
	//if (!objectManager.GetComponent<ObjectInfo>(objectManager.GetEnemyID()).IsActive())
	//{
	//	Destory(objectManager);
	//}
}

void EnemyBulletBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto info = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID());
	if ((info.IsActive() && info->GetAttribute() == ObjectAttribute::PlayerAttack) )
	{
		return;
	}

	if (info->GetAttribute() == ObjectAttribute::EnemyAttack)
	{
		return;
	}
	// ƒqƒbƒg‚Éˆ—‚ğ‘‚­
	objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
}

void EnemyBulletBehavior::Destory(ObjectManager& objManager)
{
	objManager.GetFactory(FactoryID::EnemyBullet).Destroy(ownerId_);
}
