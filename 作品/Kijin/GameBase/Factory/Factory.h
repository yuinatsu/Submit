#pragma once
#include <fstream>
#include "../Object/ObjectID.h"
#include "FactoryID.h"
#include "../Common/Vector3.h"

class ObjectManager;

// �I�u�W�F�N�g�̐����Ɣj�����s���N���X(�ꍇ�ɂ���Ă̓v�[�����s��)FactoryMethod�݂����Ȋ����ł�
class Factory
{
public:
	Factory(ObjectManager& objectManager);
	virtual ~Factory();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="id"> �������w�������I�u�W�F�N�g��ID </param>
	/// <param name="pos"> ���W </param>
	/// <param name="rot"> ��] </param>
	/// <returns></returns>
	virtual ObjectID Create(ObjectID id, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) { return ObjectID{}; };

	/// <summary>
	/// �j���p
	/// </summary>
	/// <param name="id"> �j������I�u�W�F�N�g��ID </param>
	virtual void Destroy(ObjectID id) {};

	/// <summary>
	/// ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> FactoryID </returns>
	virtual const FactoryID GetID(void) const = 0;

	/// <summary>
	/// ���[�h�����������H
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const bool IsLoaded(void);

protected:

	/// <summary>
	/// ���[�h����������֐�
	/// </summary>
	/// <param name="fileName"></param>
	void Load(const std::string& fileName);

	/// <summary>
	/// transform�f�[�^��ǂݍ���(�I�[�o�[���C�h���Ȃ���Γ����œǂݔ�΂�)
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="size"> �f�[�^�̃o�C�g�� </param>
	virtual void LoadTrans(std::ifstream& file, std::uint32_t size);

	/// <summary>
	/// Collider�f�[�^��ǂݍ���(�I�[�o�[���C�h���Ȃ���Γ����œǂݔ�΂�)
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="size"> �f�[�^�̃o�C�g�� </param>
	virtual void LoadCollider(std::ifstream& file, std::uint32_t size);


	/// <summary>
	/// Model�f�[�^��ǂݍ���(�I�[�o�[���C�h���Ȃ���Γ����œǂݔ�΂�)
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="size"> �f�[�^�̃o�C�g�� </param>
	virtual void LoadModel(std::ifstream& file, std::uint32_t size);

	/// <summary>
	/// Spawn�f�[�^��ǂݍ���(�I�[�o�[���C�h���Ȃ���Γ����œǂݔ�΂�)
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="size"> �f�[�^�̃o�C�g�� </param>
	virtual void LoadSpawn(std::ifstream& file, std::uint32_t size);

	/// <summary>
	/// Other�f�[�^��ǂݍ���(�I�[�o�[���C�h���Ȃ���Γ����œǂݔ�΂�)
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="size"> �f�[�^�̃o�C�g�� </param>
	virtual void LoadOther(std::ifstream& file, std::uint32_t size);

	// �I�u�W�F�N�g�}�l�[�W���[
	ObjectManager& objectManager_;
};

