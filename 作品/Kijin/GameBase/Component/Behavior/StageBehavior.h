#pragma once
#include "Behavior.h"
#include <map>
#include "../../Common/Vector2.h"

class StageBehavior :
	public Behavior
{
public:
	StageBehavior(std::map<size_t, Vector2>&& posList);
	StageBehavior();

	/// <summary>
	/// �G�̐��̃J�E���g�����炷
	/// </summary>
	/// <param name=""></param>
	void SubEnemy(ObjectID& id);

	/// <summary>
	/// �G�̍��W�̃��X�g���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::map<size_t, Vector2>& GetEnemyPosList(void) const&;
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) override;

protected:

	/// <summary>
	/// �G�̍��W���X�g���X�V����
	/// </summary>
	/// <param name="objectManager"></param>
	void UpdateEnemyPosList(ObjectManager& objectManager);

	// �G�̍��W�̃��X�g
	std::map<size_t, Vector2> enemyPosList_;
};

