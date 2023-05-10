#pragma once
#include "Factory.h"
#include "../Object/ObjectID.h"

// �v���C���[�𐶐�����N���X
class PlayerFactory :
    public Factory
{
public:
	PlayerFactory(const std::string& filePath,ObjectManager& objectManager);
	ObjectID Create(ObjectID id, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) final;
private:
	const FactoryID GetID(void) const { return FactoryID::Player; }
	void LoadTrans(std::ifstream& file, std::uint32_t size) final;
	void LoadModel(std::ifstream& file, std::uint32_t size) final;

	// ��Ńt�@�C�����瓖���蔻��Z�b�g�ł���悤��
	//void LoadCollider(std::ifstream& file, std::uint32_t size) final;

	// ���f���̃p�X
	std::string modelPath_;

	// ��]
	Vector3 rot_;

	// �g�k
	Vector3 scale_;
};

