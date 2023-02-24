#pragma once

enum class DataType
{
	Transform,			// ���W��]�g�k�f�[�^
	Collider,			// �����蔻��f�[�^
	Model,				// ���f���f�[�^�̃p�X
	Spawn,				// �X�|�[����������W�ƃf�[�^�p�X
	Spawner,			// �X�|�i�[�̈ʒu
	Other				// ���̂ق��̃p�����[�^�p
};

struct ObjectDataHeader
{
	char sig[4];
	unsigned int check;
	unsigned int version;
};

struct DataHeader
{
	DataType type;
	std::uint32_t size;
};

enum class ColDataType
{
	Box,
	Capsule,
	Sphere,
	Mesh
};
