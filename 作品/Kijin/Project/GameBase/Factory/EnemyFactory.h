#pragma once
#include "Factory.h"
#include <string>
#include <fstream>
#include "../Common/Vector3.h"
#include "../Common/SharedHandle.h"

// �G�𐶐�����N���X
class EnemyFactory :
	public Factory
{
public:
	EnemyFactory(ObjectID& stageID,const std::string& path, ObjectManager& objectManager);
private:
	ObjectID Create(ObjectID ownerID, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) final;
	const FactoryID GetID(void) const { return FactoryID::Enemy; }
	void LoadTrans(std::ifstream& file, std::uint32_t size) final;
	void LoadModel(std::ifstream& file, std::uint32_t size) final;
	void LoadCollider(std::ifstream& file, std::uint32_t size) final;
	void LoadOther(std::ifstream& file, std::uint32_t size) final;

	// ��]
	Vector3 rot_;

	// �g�k
	Vector3 scale_;

	// ���f���̃p�X
	std::string modelPath_;

	// ���炩���ߓǂݍ���ł������߂Ɏg��
	SharedModelHandle preLoadHandle_;

	// �����蔻��̒���
	Vector3 colCeter_;

	// �����蔻��̍���
	float height_;

	// �����蔻��̔��a
	float radius_;

	// �e�p�����[�^
	float hitTime_;
	float searchDist_;
	float speed_;
	float stopDist_;

	float shotInterval_;
	float burstInterval_;
	float burstNum_;

	// �X�e�[�W��ID
	ObjectID stageID_;
};

