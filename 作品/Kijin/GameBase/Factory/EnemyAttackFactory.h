#pragma once
#include "Factory.h"

class EnemyAttackFactory :
	public Factory
{
public:
	EnemyAttackFactory(ObjectManager& objectManager);
	~EnemyAttackFactory();
private:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="id"> ����������w�������I�u�W�F�N�g��ID </param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <returns></returns>
	ObjectID Create(ObjectID ownerID, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) final;

	/// <summary>
	/// �j������
	/// </summary>
	/// <param name="id"></param>
	void Destroy(ObjectID id) final;


	/// <summary>
	/// ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const FactoryID GetID(void) const final { return FactoryID::EnemyAttack; }
};

