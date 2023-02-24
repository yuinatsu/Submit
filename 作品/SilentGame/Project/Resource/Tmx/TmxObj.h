#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <list>
#include <functional>
#include <vector>
#include "../../common/MapCollMng.h"
#include "../../common/Math.h"
#include "../../common/rapidxml_utils.hpp"

using MapData = std::map<std::string, std::vector<int>>;

// �h�A�̃f�[�^(�����蔻���idx�ƒ��S���W)
using DoorData = std::pair<int, Math::Vector2>;
//using AnimData = std::pair<int, int>;									// pair<�摜���̉��Ԗڂ�, �t���[����>
//using AnimVecData = std::list<AnimData>;								// AnimData ��list�Ŋi�[�����f�[�^

// xml�f�[�^�i�[
//struct XmlItem
//{
//	std::map<std::string, std::string> item_;							// map<�A�C�e����, �v�f>
//	std::map<std::string, int> loop_;									// map<�A�j���[�V������, ���[�v�L��>
//	std::map<std::string, AnimVecData> data_;							// map<�A�j���[�V������, �A�j���[�V�����f�[�^>
//};

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTSX(std::string fileName);
	bool LoadTMX(std::string fileName);

	//XmlItem LoadXML(std::string fileName);


	// ���[���h���W�̎擾
	const Math::Vector2I& GetWorldArea(void);

	// �^�C���T�C�Y�̎擾
	const Math::Vector2I& GetTileSize(void);

	// ���C���[�T�C�Y�̎擾
	const unsigned int GetLayerSize(void);

	// GID�̎擾
	const unsigned int GetFirstGID(void);

	// �}�b�v�f�[�^�̎擾
	const MapData& GetMapData(void);

	// pos�͍��W�Ƃ����}�b�v�f�[�^���擾
	const int GetMapData(std::string layer, Math::Vector2I pos);

	// x,y�̓}�X�ڐ��Ƃ����}�b�v�f�[�^�̎擾
	const int GetMapData(std::string layer, int x, int y);

	// Collision���擾
	ColData& GetColList(void);

	/// <summary> �w��̔ԍ��̃R���W�����𖳌��ɂ��� </summary>
	/// <param name="idx"> �w��̏ꏊ </param>
	/// <return> �w��̏ꏊ </return>
	int OffCol(int idx);

	/// <summary> off�ɂ����R���W������L���ɂ��� </summary>
	/// <param name="idx"> �ǉ����� </param>
	/// <returns> �ǉ�������̔ԍ� </returns>
	int OnCol(int idx);

	/// <summary> �h�A�̃f�[�^���擾���� </summary>
	/// <param name=""> �h�A�̃f�[�^�̃��X�g�̎Q�� </param>
	/// <returns></returns>
	std::list<DoorData>& GetDoorData(void);
	
	/// <summary> �o�H�T�� </summary>
	/// <param name="x"> x�}�X�� </param>
	/// <param name="y"> y�}�X�� </param>
	/// <returns> true�̎��o�H�T���\ </returns>
	const bool GetNaviFlag(int x, int y) const;

	/// <summary> �o�H�T���\���̃t���O���Z�b�g���� </summary>
	/// <param name="x"> x�}�X�� </param>
	/// <param name="y"> y�}�X�� </param>
	/// <param name="flag"> �t���O </param>
	void SetNaviFlag(int x, int y, bool flag);

	/// <summary> �o�H�T���\���̃t���O���Z�b�g���� </summary>
	/// <param name="pos"> ���W </param>
	/// <param name="flag"> �t���O </param>
	void SetNaviFlag(const Math::Vector2& pos, bool flag);

	void SetNaviFlag(const Math::Vector2& pos, float r, bool flag);



private:

	/// <summary> �}�b�v�̃Z�b�g </summary>
	/// <param name=""></param>
	/// <returns> ������true���s��false </returns>
	bool SetMap(void);

	bool SetCol(rapidxml::xml_node<>* mapNode);


	bool CheckTiledVersion(rapidxml::xml_node<>* node);


	std::map<std::string, int> version_;

	// TMX
	rapidxml::xml_document<> tmxDoc_;

	// �擪��ID
	unsigned int firstGID_;

	// ���C���[��
	unsigned int layerSize_;

	// �}�b�v�̃`�b�v��
	Math::Vector2I worldArea_;

	// �^�C���̃T�C�Y
	Math::Vector2I tileSize_;

	// �}�b�v�f�[�^
	MapData mapData_;


	ColData colList_;				// �v���C���[�ƃG�l�~�[�ǂ���ɂ���������R���W�����f�[�^
	
	ColData offColList_;			// ���ݖ����ɂȂ��Ă��铖���蔻��
	
	std::list<DoorData> doorList_;				// �h�A�̃f�[�^



	// ���݂̃}�X�ڂ��o�H�T���\����ێ�
	std::vector<std::vector<bool>> naviNodeFlag_;


	// TSX
	rapidxml::xml_document<> tsxDoc_;

};

