#include "PlayerAttackBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../../Factory/PlayerAttackFactory.h"

#include "../../Common/Debug.h"
#include "PlayerBehavior.h"
#include "EnemyBehavior.h"
#include "../../UI/UiID.h"


PlayerAttackBehavior::PlayerAttackBehavior() :
	combo_{-1}, attackTime_{0.0f}, attackStartTime_{0.0f}, animTime_{0.0f}, freameIdx_{-1},update_{nullptr}
{
}

void PlayerAttackBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	
	transform_ = objectManager.GetComponent<Transform>(ownerId_);


	// çUåÇÇÃÉ{Å[ÉìÇÃç¿ïWÇ…çáÇÌÇπÇÈ
	auto pos = MV1GetFramePosition(*playerModel_->GetHandle(), freameIdx_);
	transform_->Pos() = Vector3{ pos.x, pos.y, pos.z };
	//transform_->SetRotationMatrix(MGetRotElem(MV1GetFrameLocalMatrix(*playerModel_->GetHandle(), freameIdx_)));


	//DebugLog("x=", t->pos_.x, "y=", t->pos_.y, "z=", t->pos_.z);
	(this->*update_)(objectManager, delta);
	//DebugLog(animTime_);
}

void PlayerAttackBehavior::Begin(ObjectManager& objectManager)
{
	auto player = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	playerModel_ = objectManager.GetComponent<ModelRender>(objectManager.GetPlayerID());
	transform_ = objectManager.GetComponent<Transform>(ownerId_);

	frameName_ = L"Blade-R";
	freameIdx_ = MV1SearchFrame(*playerModel_->GetHandle(), frameName_.c_str());
	auto pos = MV1GetFramePosition(*playerModel_->GetHandle(), freameIdx_);
	transform_->Pos() = player->GetPos();
	
	collider_ = objectManager.GetComponent<Collider>(ownerId_);
	collider_->SetHitFunc(std::bind(&PlayerAttackBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	collider_->SetActiveFlag(false);
	attackTime_ = 2.0f;

	update_ = &PlayerAttackBehavior::UpdateNonAttack;
}

void PlayerAttackBehavior::End(ObjectManager& objectManager)
{
	attackStartTime_ = 0.0f;
}

void PlayerAttackBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
}

void PlayerAttackBehavior::Destory(ObjectManager& objManager)
{
	objManager.GetFactory(FactoryID::PlayerAttack).Destroy(ownerId_);
}

void PlayerAttackBehavior::UpdateAttack(ObjectManager& objectManager, float delta)
{
	attackTime_ -= delta;
	if (attackTime_ <= 0.0f)
	{
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
		DebugLog("çUåÇèIóπ");
	}
}

void PlayerAttackBehavior::UpdateNonAttack(ObjectManager& objectManager, float delta)
{
	attackStartTime_ -= delta;
	if (attackStartTime_ <= 0.0f)
	{
		update_ = &PlayerAttackBehavior::UpdateAttack;
		collider_->SetActiveFlag(true);
	}
}
