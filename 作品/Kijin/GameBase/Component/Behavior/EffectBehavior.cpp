#include "EffectBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"

void EffectBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	
	if (effectRender_->IsEnd())
	{
		// 再生終了時に自分自身を破棄する
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
	}
}

void EffectBehavior::Begin(ObjectManager& objectManager)
{
	effectRender_ = objectManager.GetComponent<EffectRender>(ownerId_);
}
