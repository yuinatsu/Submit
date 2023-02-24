#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "MapData.h"

MapData::MapData(const std::string& fileName)
{
	Init(fileName);
}

MapData::~MapData()
{
}

const std::vector<int>& MapData::GetMapData(MAP_LAYER layer)
{
	return mapData_[mapKey_[layer]].first;
}

const int MapData::GetMapChipData(MAP_LAYER layer,const int& x, const int& y)
{
	const auto& tmpMapSize = mapData_[mapKey_[layer]].second.mapSize;
	int point = static_cast<size_t>(x) + static_cast<size_t>(y) * tmpMapSize.x;
	if (point < 0 && point > mapData_[mapKey_[layer]].first.size())
	{
		return -1;
	}
	return mapData_[mapKey_[layer]].first[point];
}

const int MapData::GetMapChipData(MAP_LAYER layer, const Vector2& pos)
{
	const auto& tmpChipSize = mapData_[mapKey_[layer]].second.chipSize;
	auto tmpPos = pos / tmpChipSize;
	return GetMapChipData(layer, tmpPos.x, tmpPos.y);
}

const int MapData::GetMapChipData(MAP_LAYER layer, const Vector2Flt& pos)
{
	// const持ちをcastできないため、コピーでもいい気はする
	Vector2Flt tmp = pos;
	return GetMapChipData(layer, static_cast<Vector2>(tmp));
}

bool MapData::CheckMapChip(const Vector2& pos)
{
	int re = -1;
	for (auto layer : MAP_LAYER())
	{
		re = GetMapChipData(layer, pos);
		if (re != -1)
		{
			return false;
		}
	}
	return true;
}

const mapInfo& MapData::GetMapInfo(MAP_LAYER layer)
{
	return mapData_[mapKey_[layer]].second;
}

const ColData& MapData::GetColData(void)
{
	return collisionvec_;
}

bool MapData::Init(const std::string& fileName)
{
	auto reedTmx = [&](const std::string& fileName)
	{
		std::unique_ptr<Loader::TmxLoader> loader = std::make_unique<Loader::TmxLoader>(fileName.c_str());
		int id = 0;
		for (const auto& tmp : loader->GetmapStr())
		{
			auto info = loader->GetMapInfo();
			// 取り出されるstringの一時保存先
			std::string mapStr;
			// streamに変換
			std::stringstream str(tmp.data);
			// アクセスしやすいようにenumClassに対応したstringを格納
			mapKey_[static_cast<MAP_LAYER>(id)] = tmp.name;
			id++;
			// 一時保存先
			std::vector<int> vec;
			vec.reserve(static_cast<size_t>(info.mapSize.x) * info.mapSize.y);
			while (std::getline(str, mapStr, ','))
			{
				vec.push_back(atoi(mapStr.c_str()) - info.firstGID);
			}
			mapData_[tmp.name] = { vec,info };
		}
		const auto& dataS=loader->GetColDataS();
		for (const auto& str : dataS)
		{
			collisionvec_.emplace_back(
				Vector2{ std::atoi(str.x.c_str()),std::atoi(str.y.c_str()) },
				Vector2{ std::atoi(str.width.c_str()),std::atoi(str.height.c_str()) }
			);
		}
	};
	reedTmx(fileName);
	return true;
}

MAP_LAYER begin(MAP_LAYER)
{
	return MAP_LAYER::BLOCK;
}

MAP_LAYER end(MAP_LAYER)
{
	return MAP_LAYER::MAX;
}

MAP_LAYER operator++(MAP_LAYER& layer)
{
	if (layer != MAP_LAYER::MAX)
	{
		layer = static_cast<MAP_LAYER>(std::underlying_type<MAP_LAYER>::type(layer) + 1);
	}
	return layer;
}

MAP_LAYER operator*(MAP_LAYER layer)
{
	return layer;
}
