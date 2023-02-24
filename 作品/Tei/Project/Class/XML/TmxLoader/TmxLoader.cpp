#include <iostream>
#include <cassert>
#include "TmxLoader.h"
#include "../RapidXml/rapidxml.hpp"
#include "../RapidXml/rapidxml_utils.hpp"
#include "../../../_debug/_DebugConOut.h"

Loader::TmxLoader::TmxLoader()
{
	VersionMap();			// �o�[�W�����̓o�^
}

Loader::TmxLoader::TmxLoader(const char* filename)
{
	VersionMap();			// �o�[�W�����̓o�^
	TmxLoad(filename);		// tmx�t�@�C�������[�h
}

Loader::TmxLoader::~TmxLoader()
{
}

void Loader::TmxLoader::VersionMap(void)
{
	// �Ή��o�[�W����
	version_["1.6.0"] = 1;
	version_["1.7.0"] = 1;
}

bool Loader::TmxLoader::TmxLoad(std::string filename)
{
	// tmx�֘A�̏���
	rapidxml::file<> file(filename.c_str());
	TmxDoc_.parse<0>(file.data());

	// �ŏ��̃m�[�h�擾
	tmxNode_ = TmxDoc_.first_node("map");
	if (!tmxNode_)
	{
		return false;
	}
	// �o�[�W�����Ή����Ă��邩�m�F
	if (version_.count(tmxNode_->first_attribute("tiledversion")->value()) == 0)
	{
		return false;
	}

	info_.firstGID = std::atoi(tmxNode_->first_node("tileset")->first_attribute("firstgid")->value());

	// �}�b�v�̑S�̃T�C�Y�i�[
	info_.mapSize.x = std::atoi(tmxNode_->first_attribute("width")->value());
	info_.mapSize.y = std::atoi(tmxNode_->first_attribute("height")->value());
	info_.nextLayer = std::atoi(tmxNode_->first_attribute("nextlayerid")->value());

	// �f�[�^�̊i�[����
	mapStr_.resize(GetLayerSize());

	auto itrLayer = mapStr_.begin();

	for (rapidxml::xml_node<>* layer = tmxNode_->first_node("layer"); layer != nullptr; layer = layer->next_sibling("layer"))
	{
		itrLayer->name = layer->first_attribute("name")->value();
		auto data = layer->first_node("data")->first_node();
		itrLayer->data = data->value();
		++itrLayer;
	}

	// Tsx�p�̃p�X�Â���

	auto source = tmxNode_->first_node("tileset")->first_attribute("source")->value();

	std::string pass = filename.substr(0, filename.find_last_of("/") + 1);
	LoadColData();
	return TsxLoad(pass + source);
}

bool Loader::TmxLoader::TsxLoad(std::string filename)
{
	// ����
	rapidxml::file<> file(filename.c_str());
	TsxDoc_.parse<0>(file.data());

	// Tmx�Ŋm�F���Ă邯�ǈꉞ�m�F
	if (version_.count(tmxNode_->first_attribute("tiledversion")->value()) == 0)
	{
		assert(!"TiledVersion���Ή�");
		return false;
	}

	tsx_orign_node_ = TsxDoc_.first_node("tileset");
	if (!tsx_orign_node_)
	{
		assert(!"Tsx��first_node(tileset)���Ȃ�");
		return false;
	}
	// �}�b�v�֘A �ׂ����� ���ɏȗ����ł��Ȃ��񂶂�Ȃ����Ȋ� ����Ȃ�string��ł��Ȃ��Ă����悤�ɂ�����
	info_.chipSize.x = std::atoi(tsx_orign_node_->first_attribute("tilewidth")->value());
	info_.chipSize.y = std::atoi(tsx_orign_node_->first_attribute("tileheight")->value());
	info_.imageAllNum = std::atoi(tsx_orign_node_->first_attribute("tilecount")->value());
	info_.columnsNum = std::atoi(tsx_orign_node_->first_attribute("columns")->value());
	info_.lineNum = info_.imageAllNum / info_.columnsNum;
	info_.key = tsx_orign_node_->first_attribute("name")->value();
	info_.imageStr = tsx_orign_node_->first_node("image")->first_attribute("source")->value();
	info_.imageSize.x = std::atoi(tsx_orign_node_->first_node("image")->first_attribute("width")->value());
	info_.imageSize.y = std::atoi(tsx_orign_node_->first_node("image")->first_attribute("height")->value());

	return true;
}

int Loader::TmxLoader::GetLayerSize(void)
{
	if (tmxNode_ == nullptr)
	{
		return 0;
	}
	return (std::atoi(tmxNode_->first_attribute("nextlayerid")->value()) - 1);
}

bool Loader::TmxLoader::LoadColData(void)
{
	auto objectGroup = tmxNode_->first_node("objectgroup");
	if (!objectGroup)
	{
		assert(!"objectGroup���Ȃ�");
		return false;
	}
	for (rapidxml::xml_node<>* object = objectGroup->first_node("object"); object != nullptr; object = object->next_sibling("object"))
	{
		colDataS data;
		data.x = object->first_attribute("x")->value();
		data.y = object->first_attribute("y")->value();
		data.width = object->first_attribute("width")->value();
		data.height = object->first_attribute("height")->value();
		colDataSvec_.emplace_back(data);
	}
	return true;
}

const mapStr Loader::TmxLoader::GetmapStr(void)
{
	return mapStr_;
}

const std::string Loader::TmxLoader::GetMapKey(void)
{
	return info_.key;
}

const mapInfo Loader::TmxLoader::GetMapInfo(void)
{
	return info_;
}

const std::vector <colDataS>& Loader::TmxLoader::GetColDataS(void)
{
	return colDataSvec_;
}


