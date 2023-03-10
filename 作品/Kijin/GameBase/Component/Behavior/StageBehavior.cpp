#include "StageBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../../Scene/GameScene.h"

StageBehavior::StageBehavior() :
	enemyCount_{0u}
{
}

void StageBehavior::SubEnemy(void)
{
	enemyCount_--;
}

void StageBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (enemyCount_ <= 0)
	{
		// 敵をすべて倒したのでシーンを変える
		scene.ChangeSceneID(SceneID::Result);
		dynamic_cast<GameScene&>(scene).SetResult(ResultAttribute::Clear);
	}
}
