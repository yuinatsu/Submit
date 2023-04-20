#include "EnemyBulletBehavior.h"
#include "../../Factory/EnemyBullet.h"
#include "../Info/ObjectInfo.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/Debug.h"
#include "EnemyBehavior.h"
#include "../../Common/SoundPross.h"
constexpr float speed_{600.0f};

void EnemyBulletBehavior::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Scale() = Vector3{10.0f,10.0f,10.0f};
	collider_ = objectManager.GetComponent<SphereCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBulletBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	startPos_ = transform_->GetPos();
	lpSooundPross.PlayBackSound(SOUNDNAME_SE::enemyAttack,false);
}

void EnemyBulletBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// �O�������Ɉړ�
	transform_->Pos() += transform_->GetForward() * (speed_ * delta);
	auto info = objectManager.GetComponent<ObjectInfo>(ownerId_);


	if ((transform_->GetPos() - startPos_).SqMagnitude() >= Square(5000.0f))
	{
		// ���̋����ɂȂ����玩�����g��j������
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
	}
}

void EnemyBulletBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto info = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID());
	if ((info.IsActive() && info->GetAttribute() == ObjectAttribute::PlayerAttack) )
	{
		// �v���C���[�̍U������ɓ���������������߂�
		return;
	}

	if (info->GetAttribute() == ObjectAttribute::EnemyAttack)
	{
		// �G�̋ߐڍU���ɓ�����������������߂�
		return;
	}

	// �q�b�g���ɏ���������
	objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
}

void EnemyBulletBehavior::Destory(ObjectManager& objManager)
{
	objManager.GetFactory(FactoryID::EnemyBullet).Destroy(ownerId_);
}
