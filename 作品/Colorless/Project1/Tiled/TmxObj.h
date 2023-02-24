#pragma once
#include <map>
#include <list>
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../class/common/Vector2.h"
#include "../class/common/ColorID.h"
#include "../class/common/Collision.h"

using MapData = std::map<std::string, std::vector<int>>;				// map<���C���[��, �i�[�z��>
using AnimData = std::pair<int, int>;									// pair<�摜���̉��Ԗڂ�, �t���[����>
using AnimVecData = std::list<AnimData>;								// AnimData �� vector�Ŋi�[�����f�[�^

// �A�C�e���f�[�^�i�[(xml)
struct XmlItem
{
	std::map<std::string, std::string> item_;							// map<�A�C�e����, �v�f>
	std::map<std::string, int> loop_;									// map<�A�j���[�V������, ���[�v�L��>
	std::map<std::string, AnimVecData> data_;							// map<�A�j���[�V������, �A�j���[�V�����f�[�^>
};

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string fileName);									// TSX�̓ǂݍ���
	bool LoadTmx(std::string fileName);									// TMX�̓ǂݍ���
	XmlItem LoadXmlItem(std::string fileName);							// XML��ǂݍ����item�����Ԃ�
	bool SetMap(void);													// �ǂݍ��񂾃}�b�v��mapdata_�Ɋi�[����

	// Get�֐�
	const MapData& GetMapData(void)const { return mapdata_; };			// Mapdata�̎擾
	const VecInt& GetMapData(std::string lay);							// Mapdata�̎擾(layerdata����w��)
	const int GetMapData(std::string lay, int x, int y);				// mapdata�̒��g�̎擾(mapdata���̎w�肵���ʒu)
	const int GetMapData(std::string lay, Vector2F pos);				// mapdata�̒��g�̎擾(pos����mapdata�̈ʒu���v�Z)
	const bool GetMapDataCheck(Vector2F pos);							// block�̗L���̎擾(pos����mapdata�̈ʒu���v�Z)
	const Vector2& GetWorldArea(void)const { return worldArea_; };		// �`�b�v�S�̂̃T�C�Y�̎擾
	const Vector2& GetTileSize(void)const { return tileSize_; };		// �`�b�v1�̃T�C�Y�̎擾
	const unsigned int GetFirstGid(void)const { return firstGID_; };	// �}�b�v�`�b�v�̊J�n�ԍ��̎擾
	const unsigned int GetLayerSize(void)const { return layerSize_; };	// ���C���[���̎擾
	const ColList& GetColList(void);									// �R���W�������X�g�̎擾

	const void ClearMapData(void);										// �}�b�v�f�[�^���N���A
private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);					// �o�[�W�����̊m�F
	std::map<std::string, int> version_;								// �o�[�W�����̑Ή��L��

	// TMX
	rapidxml::xml_document<> tmxDoc_;									// TMX�̃h�L�������g
	std::map<std::string, std::string> tmxitem_;						// attribute�����o������i�[����ϐ�
	unsigned int firstGID_;												// �}�b�v�`�b�v�̊J�n�ԍ�
	unsigned int layerSize_;											// ���C���[��
	Vector2 worldArea_;													// ���C���[�̃`�b�v�S�̂̃T�C�Y
	Vector2 tileSize_;													// �`�b�v1�̃T�C�Y
	MapData mapdata_;													// �}�b�v�f�[�^�̊i�[��
	ColList collist_;													// �R���W�����̊i�[���X�g

	// TSX
	rapidxml::xml_document<> tsxDoc_;									// TSX�̃h�L�������g
	std::map<std::string, std::string> tsxitem_;						// attribute�����o������i�[����ϐ�

	// �A�C�e��
	XmlItem xml_;
};

