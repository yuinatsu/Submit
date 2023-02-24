#pragma once
#include "Behavior.h"

class StageBehavior :
	public Behavior
{
public:
	StageBehavior();

	/// <summary>
	/// �G�̐��̃J�E���g�����炷
	/// </summary>
	/// <param name=""></param>
	void SubEnemy(void);

	/// <summary>
	/// �G�̐����Z�b�g
	/// </summary>
	/// <param name="count"></param>
	void SetEnemyCount(unsigned int count)
	{
		enemyCount_ = count;
	}
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	// �G�̐�
	unsigned int enemyCount_;
};

