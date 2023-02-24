#include <DxLib.h>
#include "TmxObj.h"
#include "../class/Obj/Player.h"
#include "../class/common/ImageMng.h"
#include "../_debug/_DebugConOut.h"


using rapid_node = rapidxml::xml_node<>*;
using rapid_atr = rapidxml::xml_attribute<>*;


TmxObj::TmxObj():firstGID_(0),layerSize_(0)
{
	// 使用可能なバージョン
	version_["1.6.0"] = 1;
	version_["1.7.0"] = 1;
	lpColorMng.Init();
}


TmxObj::~TmxObj()
{
}


bool TmxObj::LoadTsx(std::string fileName)
{
	rapidxml::file<> xmlFile = fileName.c_str();
	tsxDoc_.parse<0>(xmlFile.data());
	// tilesetノードの取り出し
	auto tileset = tsxDoc_.first_node("tileset");			

	// バージョンがkeyに存在するかの確認
	if (!CheckTiledVersion(tileset))						
	{
		return false;
	}
	for (auto n = tsxDoc_.first_node(); n != nullptr; n = n->first_node())
	{
		// ノードがあれば名前を書きだす
		if (n)												
		{
			if (n->name_size()) 
			{
				TRACE("%s→ ", n->name());
			}
		}
		// attrbuteの取り出し
		auto atr = n->first_attribute();					
		// attrbuteが存在する間処理
		while (atr)											
		{
			// 名前の書き出し
			if (atr->name_size())							
			{
				TRACE("%s ", atr->name());
			}
			// 要素の書き出し
			TRACE("%s ", atr->value());						
			// mapのkeyにnameを使って値を入れる
			tsxitem_[atr->name()] = atr->value();			
			// 次のattributeを呼ぶ
			atr = atr->next_attribute();					
		}
	}

	// ソース名の取り出し
	std::string source = tsxitem_["source"].substr(tsxitem_["source"].find_first_of("/") + 1);
	// タイル数の取り出し
	int count = std::atoi(tsxitem_["tilecount"].c_str()) / std::atoi(tsxitem_["columns"].c_str());

	// 画像の読み込み
	lpImageMng.GetID(
		source,
		tsxitem_["name"],
		Vector2(std::atoi(tsxitem_["tilewidth"].c_str()), std::atoi(tsxitem_["tileheight"].c_str())),
		Vector2(std::atoi(tsxitem_["columns"].c_str()),count)
	);
	return true;
}


bool TmxObj::LoadTmx(std::string fileName)
{
	rapidxml::file<> xmlFile = fileName.c_str();
	tmxDoc_.parse<0>(xmlFile.data());

	// バージョンがkeyに存在するかの確認
	auto mapnode = tmxDoc_.first_node("map");											
	if (!CheckTiledVersion(mapnode))
	{
		return false;
	}

	// TSXがあるファイルまでのパスの取得
	std::string sourcestr = fileName.substr(0, fileName.find_last_of("/") + 1);			
	sourcestr += mapnode->first_node("tileset")->first_attribute("source")->value();
	// TSXが読み込み確認
	if (!LoadTsx(sourcestr))															
	{
		return false;
	}
	auto mapatr = [&](std::string str) {
		return std::atoi(mapnode->first_attribute(str.c_str())->value());
	};

	worldArea_ = Vector2(mapatr("width"), mapatr("height"));
	tileSize_ = Vector2(mapatr("tilewidth"), mapatr("tileheight"));
	layerSize_ = mapatr("nextlayerid") - 1;
	firstGID_ = std::atoi(mapnode->first_node("tileset")->first_attribute("firstgid")->value());


	if (!SetMap()) 
	{
		return false;
	}
	return true;
}


XmlItem TmxObj::LoadXmlItem(std::string fileName)
{
	rapidxml::file<> xmlFile = fileName.c_str();
	tsxDoc_.parse<0>(xmlFile.data());

	// imageノードの取り出し
	auto image = tsxDoc_.first_node();						
	// imageの情報
	if (image != nullptr)									
	{
		for (auto atr = image->first_attribute(); atr != nullptr; atr = atr->next_attribute()) 
		{
			// mapのkeyにnameを使って値を入れる
			xml_.item_[atr->name()] = atr->value();			
		}
		std::string dirname;
		// アニメーション情報
		for (auto animation = image->first_node("animation"); animation != nullptr; animation = animation->next_sibling())		
		{
			if (animation) 
			{
				for (auto atr = animation->first_attribute(); atr != nullptr; atr = atr->next_attribute()) 
				{
					std::string name = atr->name();
					if (name == "dirname") 
					{
						dirname = atr->value();
					}
					else if (name == "loop") 
					{
						xml_.loop_[dirname] = std::atoi(atr->value());
					}
				}
			}
			std::pair<int, int> tmppair;
			for (auto datasib = animation->first_node(); datasib != nullptr; datasib = datasib->next_sibling()) 
			{
				if (datasib) {
					for (auto atr = datasib->first_attribute(); atr != nullptr; atr = atr->next_attribute()) 
					{
						std::string name = atr->name();
						if (name == "count") 
						{
							tmppair.first = std::atoi(atr->value());
						}
						else if (name == "frame")
						{
							tmppair.second = std::atoi(atr->value());
						}
					}
					xml_.data_[dirname].emplace_back(tmppair);
				}
			}
		}
	}

	// 全体のチップサイズ
	std::string source = xml_.item_["source"].substr(xml_.item_["source"].find_first_of("/") + 1);								

	// 画像の読み込み
	lpImageMng.GetID(
		source,
		xml_.item_["name"],
		Vector2(std::atoi(xml_.item_["width"].c_str()), std::atoi(xml_.item_["height"].c_str())),
		Vector2(std::atoi(xml_.item_["divwidth"].c_str()), std::atoi(xml_.item_["divheight"].c_str()))
	);
	return xml_;
}


bool TmxObj::SetMap(void)
{
	// mapノード読み込み
	auto mapnode = tmxDoc_.first_node("map");					
	if (mapnode == nullptr)
	{
		TRACE("mapノード取得エラー\n");
		return false;
	}
	// layerノード読み込み
	auto laynode = mapnode->first_node("layer");				
	if (laynode == nullptr) 
	{
		TRACE("layerノード取得エラー");
		return false;
	}
	// バージョンがkeyに存在するかの確認
	if (!CheckTiledVersion(mapnode))							
	{
		return false;
	}

	std::map<std::string, std::pair<MapData::iterator, bool>> layer;
	auto name = [&]() { 
		std::string node = laynode->first_attribute("name")->value();
		return node; 
	};

	do
	{
		std::string node = laynode->first_attribute("name")->value();
		// colデータは後でまとめて格納
		if (node == "col" || node == "black col" || node == "door col" || node == "crystal col" || 
			node == "light blue col" || node == "purple col" || node == "yellow col" || node == "green col" || node == "red col" || node == "orange col" || node == "blue col" || 
			node == "light blue item col" || node == "purple item col" || node == "yellow item col" || node == "green item col" || node == "red item col" || node == "orange item col" || node == "blue item col")										// コリジョンだったら別処理
		{
			laynode = laynode->next_sibling();
			continue;
		}

		// csvを取り出す
		layer[name()] = mapdata_.try_emplace(laynode->first_attribute("name")->value());			
		if (layer[name()].second)
		{
			layer[name()].first->second.resize(static_cast<__int64>(worldArea_.x) * worldArea_.y);
		}

		// csvをint型に変えて格納する
		auto data = laynode->first_node("data")->value();		
		std::string str = data;
		int strsize = static_cast<int>(str.size());
		for (int i = 0, wpos = 0, old = 0, nexti = 1; str.size() > i; i++, nexti = i + 1)
		{
			if (str[i] == '\r' || str[i] == '\n' || str[i] == '\r\n' || str[i] == ',')
			{
				continue;
			}
			else
			{
				std::string tmp;
				tmp.resize(1);
				tmp[0] = str[i];
				// 背景色と同色のオブジェクトは非表示
				if (((node == "light blue" && !lpColorMng.GetColor(Color::lightBlue)) && (node == "light blue" && !lpColorMng.GetItemFlag(Color::lightBlueItem))) ||
					((node == "purple" && !lpColorMng.GetColor(Color::purple))		  && (node == "purple" && !lpColorMng.GetItemFlag(Color::purpleItem))) ||
					((node == "yellow" && !lpColorMng.GetColor(Color::yellow))		  && (node == "yellow" && !lpColorMng.GetItemFlag(Color::yellowItem))) ||
					((node == "green" && !lpColorMng.GetColor(Color::green))		  && (node == "green" && !lpColorMng.GetItemFlag(Color::greenItem))) ||
					((node == "red" && !lpColorMng.GetColor(Color::red))			  && (node == "red" && !lpColorMng.GetItemFlag(Color::redItem))) ||
					((node == "orange" && !lpColorMng.GetColor(Color::orange))		  && (node == "orange" && !lpColorMng.GetItemFlag(Color::orangeItem))) ||
					((node == "blue" && !lpColorMng.GetColor(Color::blue))			  && (node == "blue" && !lpColorMng.GetItemFlag(Color::blueItem))) ||
					(((node == "light blue item" && !lpColorMng.GetColor(Color::lightBlue)) && (node == "light blue item" && !lpColorMng.GetItemFlag(Color::lightBlueItem))) || (node == "light blue item" && !lpColorMng.GetItemFlag(Color::lightBlueItem))) ||
					(((node == "purple item" && !lpColorMng.GetColor(Color::purple))		&& (node == "purple item" && !lpColorMng.GetItemFlag(Color::purpleItem)))		 || (node == "purple item" && !lpColorMng.GetItemFlag(Color::purpleItem))) ||
					(((node == "yellow item" && !lpColorMng.GetColor(Color::yellow))		&& (node == "yellow item" && !lpColorMng.GetItemFlag(Color::yellowItem)))		 || (node == "yellow item" && !lpColorMng.GetItemFlag(Color::yellowItem))) ||
					(((node == "green item" && !lpColorMng.GetColor(Color::green))			&& (node == "green item" && !lpColorMng.GetItemFlag(Color::greenItem)))			 || (node == "green item" && !lpColorMng.GetItemFlag(Color::greenItem))) ||
					(((node == "red item" && !lpColorMng.GetColor(Color::red))				&& (node == "red item" && !lpColorMng.GetItemFlag(Color::redItem)))				 || (node == "red item" && !lpColorMng.GetItemFlag(Color::redItem))) ||
					(((node == "orange item" && !lpColorMng.GetColor(Color::orange))		&& (node == "orange item" && !lpColorMng.GetItemFlag(Color::orangeItem)))		 || (node == "orange item" && !lpColorMng.GetItemFlag(Color::orangeItem))) ||
					(((node == "blue item" && !lpColorMng.GetColor(Color::blue))			&& (node == "blue item" && !lpColorMng.GetItemFlag(Color::blueItem)))			 || (node == "blue item" && !lpColorMng.GetItemFlag(Color::blueItem))))
				{
					if (str.size() > nexti&& str[nexti] != ',' && str[nexti] != '\r' && str[nexti] != '\n' && str[nexti] != '\r\n')
					{
						tmp += str[nexti];
						i++;
					}
					layer[name()].first->second[wpos] = 0;
					wpos++;
				}
				else
				{
					if (str.size() > nexti&& str[nexti] != ',' && str[nexti] != '\r' && str[nexti] != '\n' && str[nexti] != '\r\n')
					{
						tmp += str[nexti];
						i++;
					}
					layer[name()].first->second[wpos] = std::atoi(tmp.c_str());
					wpos++;
				}
			}
		}
		// colデータ格納
		for (auto colnode = mapnode->first_node("objectgroup"); colnode != nullptr; colnode = colnode->next_sibling())
		{
			std::string node = colnode->first_attribute("name")->value();
			auto color = Color::max;
			if (node == "black col")
			{
				color = Color::black;
			}
			else if (node == "door col")
			{
				color = Color::door;
			}
			else if (node == "light blue col")
			{
				color = Color::lightBlue;
			}
			else if (node == "purple col")
			{
				color = Color::purple;
			}
			else if (node == "yellow col")
			{
				color = Color::yellow;
			}
			else if (node == "green col")
			{
				color = Color::green;
			}
			else if (node == "red col")
			{
				color = Color::red;
			}
			else if (node == "orange col")
			{
				color = Color::orange;
			}
			else if (node == "blue col")
			{
				color = Color::blue;
			}
			else if (node == "light blue item col")
			{
				if(lpColorMng.GetItemFlag(Color::lightBlueItem))
				{
					color = Color::lightBlueItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "purple item col")
			{
				if (lpColorMng.GetItemFlag(Color::purpleItem))
				{
					color = Color::purpleItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "yellow item col")
			{
				if (lpColorMng.GetItemFlag(Color::yellowItem))
				{
					color = Color::yellowItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "green item col")
			{
				if (lpColorMng.GetItemFlag(Color::greenItem))
				{
					color = Color::greenItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "red item col")
			{
				if (lpColorMng.GetItemFlag(Color::redItem))
				{
					color = Color::redItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "orange item col")
			{
				if (lpColorMng.GetItemFlag(Color::orangeItem))
				{
					color = Color::orangeItem;
				}
				else
				{
					color = Color::max;
				}
			}
			else if (node == "blue item col")
			{
				if (lpColorMng.GetItemFlag(Color::blueItem))
				{
					color = Color::blueItem;
				}
				else
				{
					color = Color::max;
				}
			}
			for (auto objnode = colnode->first_node("object"); objnode != nullptr; objnode = objnode->next_sibling())
			{
				collist_.push_back(
					std::tuple<Vector2F, Vector2F, Color>(
						Vector2F(objnode->first_attribute("x")->value(), objnode->first_attribute("y")->value()),
						Vector2F(objnode->first_attribute("width")->value(), objnode->first_attribute("height")->value()),
						color
					)
				);
			}
		}
		laynode = laynode->next_sibling();
	} 
	while (laynode);

	return true;
}


const VecInt& TmxObj::GetMapData(std::string lay)
{
	return mapdata_[lay];
}


const int TmxObj::GetMapData(std::string lay,int x,int y)
{
	if (mapdata_.find(lay) == mapdata_.end()) 
	{
		return 0;
	}
	int point = x + (worldArea_.x * y);
	if (point >= 0 && point < mapdata_[lay].size())
	{
		return mapdata_[lay][point];
	}
	else
	{
		return 0;
	}
	return 0;
}


const int TmxObj::GetMapData(std::string lay, Vector2F pos)
{
	Vector2 chip = (static_cast<Vector2>(pos) / tileSize_);

	return GetMapData(lay,chip.x,chip.y);
}


const bool TmxObj::GetMapDataCheck(Vector2F pos)
{
	Vector2 p;
	p.x = static_cast<int>(pos.x);
	p.y = static_cast<int>(pos.y);

	Vector2 chip = (p / tileSize_);
	int point = chip.x + (worldArea_.x * chip.y);
	if (point < 0)
	{
		return true;
	}
	for (auto& map : mapdata_)
	{
		if (map.second.size() > point && mapdata_[map.first][point]) 
		{
			return true;
		}
	}
	return false;
}


const ColList& TmxObj::GetColList(void)
{
	return collist_;
}


const void TmxObj::ClearMapData(void)
{
	TRACE("削除\n");
	mapdata_.clear();
	collist_.clear();
}


bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node)
{
	if (node == nullptr) 
	{
		return false;
	}
	// タイルセットノード中のバージョン情報の取り出し
	auto tiledversion = node->first_attribute("tiledversion");				
	// バージョン番号の取り出し
	auto data = tiledversion->value();										
	auto version = node->first_attribute();

	// バージョンがkeyに存在するかの確認
	if (!version_.count(data))												
	{
		return false;
	}
	// 中身のboolを確認しtrueだったら次へ進む
	if (!version_[data])													
	{
		return false;
	}
	return true;
}
