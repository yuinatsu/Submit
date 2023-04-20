#pragma once

// �f�[�^�̎��
enum class DataType
{
	Transform,			// ���W��]�g�k�f�[�^
	Collider,			// �����蔻��f�[�^
	Model,				// ���f���f�[�^�̃p�X
	Spawn,				// �X�|�[����������W�ƃf�[�^�p�X
	Spawner,			// �X�|�i�[�̈ʒu
	Other				// ���̂ق��̃p�����[�^�p
};

// �f�[�^�t�@�C���̃w�b�_�[
struct ObjectDataHeader
{
	char sig[4];			// �V�O�l�`��
	unsigned int check;		// �`�F�b�N�p
	unsigned int version;	// �o�[�W�������
};

// �f�[�^���̃w�b�_�[
struct DataHeader
{
	DataType type;			// �^�C�v
	std::uint32_t size;		// �f�[�^�T�C�Y
};

// �����蔻��f�[�^�̎��
enum class ColDataType
{
	Box,
	Capsule,
	Sphere,
	Mesh,
};
