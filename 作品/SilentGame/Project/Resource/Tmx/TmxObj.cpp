#include "TmxObj.h"
#include <sstream>
#include "../../common/ImageMng.h"
#include "../../common/Math.h"
#include "../../common/Debug.h"
#include <DxLib.h>

using namespace Math;

TmxObj::TmxObj()
{
	version_["1.8.2"] = 1;
	firstGID_ = 0;
	layerSize_ = 0;
}

TmxObj::~TmxObj()
{
}

bool TmxObj::LoadTSX(std::string fileName)
{
	rapidxml::file<> xmlFileName = fileName.c_str();
	// ファイルを開いてデータを渡す
	tsxDoc_.parse<0>(xmlFileName.data());
	rapidxml::xml_node<>* tileset = tsxDoc_.first_node("tileset");

	if (!CheckTiledVersion(tileset))
	{
		return false;
	}
	// 子のノードを取得
	auto imageNode = tileset->first_node("image");
	// ファイル名の取得
	std::string source = imageNode->first_attribute("source")->value();
	auto Source = source.substr(source.find_first_of("/") + 1);
	// パス情報を抜いてkeyNameに入れる
	auto keyName = source.substr(source.find_last_of("/") + 1);

	// サイズの取得
	auto tilewidth = std::stoi(tileset->first_attribute("tilewidth")->value());
	auto tileheight = std::stoi(tileset->first_attribute("tileheight")->value());
	// 分割数の取得
	auto divCntX = std::stoi(tileset->first_attribute("columns")->value());
	auto divCntY = std::stoi(tileset->first_attribute("tilecount")->value()) / divCntX;;

	lpImageMng.GetID("Resource/Tmx/Image/"+Source, keyName, Vector2I{tilewidth,tileheight}, Vector2I{divCntX,divCntY});
	return true;
}

bool TmxObj::LoadTMX(std::string fileName)
{
	// ファイル読み込み
	rapidxml::file<> xmlFileName = fileName.c_str();
	// ファイルを開いてデータを渡す
	tmxDoc_.parse<0>(xmlFileName.data());
	// 先頭にmapノードの取得
	rapidxml::xml_node<>* mapNode = tmxDoc_.first_node("map");
	if (!CheckTiledVersion(mapNode))
	{
		return false;
	}
	auto tileset = mapNode->first_node("tileset");
	if (!LoadTSX(fileName.substr(0, fileName.find_last_of("/") + 1) + tileset->first_attribute("source")->value()))
	{
		return false;
	}

	firstGID_ = std::stoi(tileset->first_attribute("firstgid")->value());
	layerSize_ = std::stoi(mapNode->first_attribute("nextlayerid")->value());
	worldArea_ = Vector2I{ std::stoi(mapNode->first_attribute("width")->value()),std::stoi(mapNode->first_attribute("height")->value()) };
	tileSize_ = Vector2I{ std::stoi(mapNode->first_attribute("tilewidth")->value()),std::stoi(mapNode->first_attribute("tileheight")->value()) };
	
	

	bool rtn{ SetMap() };
	
	

	return rtn;
}



bool TmxObj::SetMap(void)
{
	rapidxml::xml_node<>* mapNode = tmxDoc_.first_node("map");

	if (mapNode == nullptr)
	{
		return false;
	}
	for (auto layerNode = mapNode->first_node("layer");
		layerNode != nullptr;
		layerNode = layerNode->next_sibling())
	{
		if (layerNode == nullptr)
		{
			return false;
		}
		std::string nodeName = layerNode->first_attribute("name")->value();
		if (nodeName == "Col1" || nodeName == "Col2" )
		{
			continue;
		}

		auto layer = mapData_.try_emplace(layerNode->first_attribute("name")->value());
		if (layer.second)
		{
			layer.first->second.resize(static_cast<size_t> (worldArea_.x) * worldArea_.y);
		}

		auto layerData = layerNode->first_node("data")->first_node();

		std::stringstream dataStream;
		dataStream << layerData->value();
		for (auto& data : layer.first->second)
		{
			std::string numStr;
			getline(dataStream, numStr, ',');
			data = atoi(numStr.c_str());
		}
	}

	naviNodeFlag_.resize(GetWorldArea().y);
	for (auto& v : naviNodeFlag_)
	{
		v.resize(GetWorldArea().x);
	}

	// 経路探索を除外する対象
	const std::vector nonNaviId{
		0,218,219,292,293, 364, 365, 366, 367, 368, 369, 
		438, 439,440,441,442,443,512,513,514,515,516,517,
		586,587,588,589,590,591,662,663,736,737
	};

	auto check = [this, &nonNaviId](int x,int y) {

		if (GetMapData("2Block", x, y) != 0)
		{
			return false;
		}
		auto rtn = GetMapData("1Floor", x, y);
		for (const auto& n : nonNaviId)
		{
			if ((rtn) == (n + firstGID_))
			{
				return false;
			}
		}
		return rtn != 0;
	};

	for (int y = 0; y < GetWorldArea().y; y++)
	{
		for (int x = 0; x < GetWorldArea().x; x++)
		{
			naviNodeFlag_[y][x] = check(x,y);
		}
	}



	return SetCol(mapNode);
}

bool TmxObj::SetCol(rapidxml::xml_node<>* mapNode)
{
	auto colNode = mapNode->first_node("objectgroup");
	while (colNode)
	{

		if (std::string{ colNode->first_attribute("name")->value() } == "Col1")
		{
			// グループ名がCol1の時
			for (auto objectNode = colNode->first_node("object");
				objectNode != nullptr;
				objectNode = objectNode->next_sibling())
			{
				
				// 座標を取る
				auto colX = std::stof(objectNode->first_attribute("x")->value());
				auto colY = std::stof(objectNode->first_attribute("y")->value());

				// コリジョンをセット
				colList_.push_back(
					{
						Vector2{ static_cast<float>(colX),static_cast<float>(colY) },     // 左上頂点
						Vector2{ static_cast<float>(atof(objectNode->first_attribute("width")->value())),static_cast<float>(atof(objectNode->first_attribute("height")->value())) }     // 幅と高さ
					}
				);
			}
		}
		if (std::string{ colNode->first_attribute("name")->value() } == "Col2")
		{
			// グループ名がCol2の時

			// 次に当たる要素数を取得
			int idx = static_cast<int>(colList_.size());

			// データを取る
			for (auto objectNode = colNode->first_node("object");
				objectNode != nullptr;
				objectNode = objectNode->next_sibling())
			{
				// 座標を取る
				auto colX = std::stof(objectNode->first_attribute("x")->value());
				auto colY = std::stof(objectNode->first_attribute("y")->value());

				auto width = std::stof(objectNode->first_attribute("width")->value());
				auto height = std::stof(objectNode->first_attribute("height")->value());

				// 中心座標を取る
				auto prop = objectNode->first_node("properties")->first_node("property");
				auto x = std::stof(prop->first_attribute("value")->value());
				prop = prop->next_sibling("property");
				auto y = std::stof(prop->first_attribute("value")->value());

				// 経路探索をつぶす
				for (float y = colY; y <= colY + height; y += static_cast<float>(tileSize_.y))
				{
					for (float x = colX; x <= colX + width; x += static_cast<float>(tileSize_.x))
					{
						SetNaviFlag(Vector2{ x,y }, false);
					}
				}

				// コリジョンとしてセット
				colList_.push_back(
					{
						Vector2{ colX, colY},	// 左上頂点
						Vector2{ width,height}											// 幅と高さ
					}
				);

				// セットしたコリジョンの番号と中心座標をドアのデータとしてセット
				doorList_.push_back(
					std::make_pair(idx, Vector2{ x,y })
				);
				idx++;
			}
		}
		// 次のノードに移動
		colNode = colNode->next_sibling();
	}
	return true;
}

const Math::Vector2I& TmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Math::Vector2I& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const unsigned int TmxObj::GetLayerSize(void)
{
	return layerSize_;
}

const unsigned int TmxObj::GetFirstGID(void)
{
	return firstGID_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const int TmxObj::GetMapData(std::string layer, Math::Vector2I pos)
{
	auto chipPos = pos / tileSize_;
	return GetMapData(layer, chipPos.x, chipPos.y);
}

const int TmxObj::GetMapData(std::string layer, int x, int y)
{
	if (mapData_.count(layer) == 0)
	{
		return 0;
	}
	int point = x + (y * worldArea_.x);
	if (point >= 0 && point < mapData_[layer].size())
	{
		return mapData_[layer][point];
	}
	else
	{
		return 0;
	}
	return 0;
}

ColData& TmxObj::GetColList(void)
{
	return colList_;
}

int TmxObj::OffCol(int idx)
{
	if (idx > static_cast<int>(colList_.size()))
	{
		return -1;
	}

	offColList_.emplace_back(colList_[idx]);
	colList_.erase(colList_.begin() + idx);
	return static_cast<int>(offColList_.size()) - 1;
}

int TmxObj::OnCol(int idx)
{
	colList_.emplace_back(offColList_[idx]);
	offColList_.erase(offColList_.begin() + idx);

	return static_cast<int>(colList_.size()) - 1;
}

std::list<DoorData>& TmxObj::GetDoorData(void)
{
	return doorList_;
}



const bool TmxObj::GetNaviFlag(int x, int y) const
{
	if (x < 0 || x >= worldArea_.x || y < 0 || y >= worldArea_.y)
	{
		return false;
	}
	return naviNodeFlag_[y][x];
}

void TmxObj::SetNaviFlag(int x, int y, bool flag)
{
	if (x < 0 || x >= worldArea_.x || y < 0 || y >= worldArea_.y)
	{
		return;
	}
	naviNodeFlag_[y][x] = flag;
}

void TmxObj::SetNaviFlag(const Math::Vector2& pos, bool flag)
{
	SetNaviFlag(static_cast<int>(pos.x) / tileSize_.x, static_cast<int>(pos.y) / tileSize_.y, flag);
}

void TmxObj::SetNaviFlag(const Math::Vector2& pos, float r, bool flag)
{
	/*auto numMax = static_cast<Math::Vector2I>(((pos + r) - pos))/tileSize_;
	auto numMin = static_cast<Math::Vector2I>(((pos - r) - pos)) / tileSize_;
	for (int x = numMin.x; x < numMax.x; x++)
	{
		for (int y = numMin.y; y < numMax.y; y++)
		{
			SetNaviFlag(x, y, flag);
		}
	}*/
	const auto halfTileSize{ tileSize_ / 2 };
	
	/*SetNaviFlag(
		pos + static_cast<Math::Vector2>(halfTileSize),
		false
	);
	SetNaviFlag(
		pos + static_cast<Math::Vector2>(halfTileSize),
		false
	);*/
	const int almR{ (static_cast<int>(r) + (tileSize_.x - 1)) & ~(tileSize_.x - 1) };
	
	const Math::Vector2I grdPos{ static_cast<Math::Vector2I>(pos) / tileSize_ };

	auto t = almR / tileSize_.x;
	for (int x = 0; x < almR/tileSize_.x ; x++)
	{
		/*SetNaviFlag(grdPos.x + x, grdPos.y, false);
		SetNaviFlag(grdPos.x - x, grdPos.y, false);*/
		for (int y = 0; y < almR / tileSize_.y; y++)
		{
			/*SetNaviFlag(
				pos + static_cast<Math::Vector2>(halfTileSize) + Math::Vector2{ static_cast<float>(tileSize_.x * x), static_cast<float>(tileSize_.y * y)},
				false
			);
			SetNaviFlag(
				pos + static_cast<Math::Vector2>(halfTileSize) - Math::Vector2{ static_cast<float>(tileSize_.x * x), static_cast<float>(tileSize_.y * y) },
				false
			);*/
			SetNaviFlag(grdPos.x + (x), grdPos.y + (y), false);
			SetNaviFlag(grdPos.x - (x), grdPos.y - (y), false);
			DebugLog(t);
		}
	}

	//for (int x = 0; x <= almR / tileSize_.x; x++)
	//{
	//	SetNaviFlag(grdPos.x + x, grdPos.y, false);
	//	SetNaviFlag(grdPos.x - x, grdPos.y, false);
	//}
	//for (int y = 0; y <= almR / tileSize_.y; y++)
	//{
	//	SetNaviFlag(grdPos.x , grdPos.y + y, false);
	//	SetNaviFlag(grdPos.x , grdPos.y - y, false);
	//}
}





bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return false;
	}
	auto tiledVersion = node->first_attribute("tiledversion");
	// virsion_の中にtiledversion->value()の数値があるかを調べる
	if (!version_.count(tiledVersion->value()))
	{
		return false;
	}
	if (!version_[tiledVersion->value()])
	{
		return false;
	}
	return true;
}
