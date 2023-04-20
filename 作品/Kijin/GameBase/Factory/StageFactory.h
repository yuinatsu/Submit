#pragma once
#include <filesystem>
#include "Factory.h"
#include <atomic>
#include <map>
#include "../Common/Vector2.h"

// �X�e�[�W���쐬����N���X
class StageFactory :
    public Factory
{
public:
	StageFactory(const std::filesystem::path& path, ObjectManager& objectManager);
	StageFactory(ObjectManager& objectManager);
private:
	const FactoryID GetID(void) const { return FactoryID::Stage; }
	
	/// <summary>
	/// ���[�h�p�֐�
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// �`���[�g���A�����[�h�p
	/// </summary>
	/// <param name=""></param>
	void LoadTutorial(void);

	/// <summary>
	/// ���[�h����������
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsLoaded(void) final;

	/// <summary>
	/// �w�i�̃X�J�C�h�[���̐���
	/// </summary>
	/// <param name="file"></param>
	void CreateSkyDome(std::ifstream& file);
	
	/// <summary>
	/// �v���C���[�̐���
	/// </summary>
	/// <param name="file"></param>
	void CreatePlayer(std::ifstream& file);

	/// <summary>
	/// �G�̐���
	/// </summary>
	/// <param name="file"></param>
	void CreateEnemy(std::ifstream& file) ;

	void LoadTrans(std::ifstream& file, std::uint32_t size) final;

	void LoadModel(std::ifstream& file, std::uint32_t size) final;

	void LoadSpawn(std::ifstream& file, std::uint32_t size) final;

	void LoadCollider(std::ifstream& file, std::uint32_t size) final;

	// ���[�h�����������̃t���O
	std::atomic_bool isLoaded_;

	// �f�[�^�t�@�C���̃p�X
	std::filesystem::path path_;

	// �X�e�[�W�̓����蔻��̃t�@�C����
	std::string stageCol_;

	// �X�e�[�W�̃I�u�W�F�N�gID
	ObjectID id_;


	// ���f���̃t�@�C����
	std::string model_;

	Vector3 pos_;
	Vector3 scale_;


	// �֐��̃}�b�v
	std::map<FactoryID, void(StageFactory::*)(std::ifstream&)> funcMap_;

	std::map<size_t, Vector2> enemyPosList_;
};

