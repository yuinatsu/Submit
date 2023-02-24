#pragma once

#include <string>
#include <vector>
#include <map>
#include "../RapidXml/rapidxml.hpp"
#include "../../common/Vector2.h"


// �f�[�^�i�[�\����
struct Layer
{
	std::string name;
	std::string data;
};

// �}�b�v�쐬���K�v���\����
struct mapInfo
{
	Vector2 mapSize = Vector2{};
	Vector2 chipSize = Vector2{};
	unsigned int firstGID = 0;
	unsigned int imageAllNum = 0;
	unsigned int columnsNum = 0;
	unsigned int lineNum = 0;
	unsigned int nextLayer = 0;
	std::string key = std::string{};
	std::string imageStr = std::string{};
	Vector2 imageSize = Vector2{};
};

struct colDataS
{
	std::string x;
	std::string y;
	std::string width;
	std::string height;
};
// ����Ȓ����Ȃ�����using���Ƃ�����
using mapStr = std::vector<Layer>;

namespace Loader
{
	class TmxLoader
	{
	public:
		TmxLoader();
		TmxLoader(const char* filename);
		bool TmxLoad(std::string filename);			// Tmx�t�@�C�����[�h�p ������TsxLoad���Ă�ł���
		const mapStr GetmapStr(void);				// ���[�h���Ă���string��n���z
		const std::string GetMapKey(void);			// Tsx���[�h���쐬���ꂽImage�փA�N�Z�X���邽�߂̃L�[
		const mapInfo GetMapInfo(void);				// �}�b�v�쐬���ɕK�v�ȏ��Z�b�g
		const std::vector <colDataS>& GetColDataS(void);			// ColDataS�̊l��
		~TmxLoader();
	private:
		void VersionMap(void);						// �Ή��o�[�W�����i�[���邽�߂����̓z
		int  GetLayerSize(void);					// ���C���[�̍ő吔�l�����邾���̓z
		bool LoadColData(void);						// colision�p�̃f�[�^�̓ǂݍ���
		bool TsxLoad(std::string filename);			// Tsx�t�@�C�������[�h TmxLoad����Ăяo�����

		rapidxml::xml_document<> TmxDoc_;
		rapidxml::xml_document<> TsxDoc_;

		rapidxml::xml_node<>* tmxNode_;		// ��ԍŏ��̃m�[�h ��{�I�ɂ�������A�N�Z�X����
		rapidxml::xml_node<>* tsx_orign_node_;

		std::map<std::string, int> version_;		// ����ʃo�[�W�����ɑΉ������ۂ����ɑΉ������o�[�W�������L�[�ɂ��ēK���Ȓl��������Ƃ��ΑΉ����Ă邩�ǂ����m�F����Ƃ��Ɋy
		mapStr mapStr_;								// �ǂݍ���string�̕ۑ��ꏊ
		std::vector<colDataS> colDataSvec_;			// �ǂݍ���Collision�p�̏��ۑ���
		mapInfo info_;								// �ǂݍ��񂾃}�b�v�쐻�p�����g���₷���`�ɕς��i�[��������
	};
}

