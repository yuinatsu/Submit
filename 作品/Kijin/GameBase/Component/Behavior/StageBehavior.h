#pragma once
#include "Behavior.h"

class StageBehavior :
	public Behavior
{
public:
	StageBehavior();

	/// <summary>
	/// 敵の数のカウントを減らす
	/// </summary>
	/// <param name=""></param>
	void SubEnemy(void);

	/// <summary>
	/// 敵の数をセット
	/// </summary>
	/// <param name="count"></param>
	void SetEnemyCount(unsigned int count)
	{
		enemyCount_ = count;
	}
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	// 敵の数
	unsigned int enemyCount_;
};

