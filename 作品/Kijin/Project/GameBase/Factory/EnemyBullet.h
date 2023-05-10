#pragma once
#include "Factory.h"
#include <forward_list>
#include "../Common/SharedHandle.h"

class EnemyBulletBehavior;
class SphereCollider;
class ObjectInfo;
class ModelRender;
class Transform;

// �G�����˂���e�𐶐�����N���X
class EnemyBulletFactory :
	public Factory
{
public:
	EnemyBulletFactory(ObjectManager& objectManager);
	~EnemyBulletFactory();
	
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="id"> ����������w�������I�u�W�F�N�g��ID </param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <returns></returns>
	ObjectID Create(ObjectID id, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) final;
	
	/// <summary>
	/// �j������
	/// </summary>
	/// <param name="id"></param>
	void Destroy(ObjectID id) final;
private:

	/// <summary>
	/// ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const FactoryID GetID(void) const { return FactoryID::EnemyBullet; }
  
	// �����Ɋւ���N���X�̃v�[��
	std::forward_list<std::unique_ptr<EnemyBulletBehavior>> behaviorPool_;

	// �n���h��
	SharedModelHandle model_;

};

