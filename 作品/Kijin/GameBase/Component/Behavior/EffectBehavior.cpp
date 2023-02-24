#include "EffectBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"

void EffectBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	
	if (effectRender_->IsEnd())
	{
		// �Đ��I�����Ɏ������g��j������
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
	}
}

void EffectBehavior::Begin(ObjectManager& objectManager)
{
	effectRender_ = objectManager.GetComponent<EffectRender>(ownerId_);
}
