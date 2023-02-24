#pragma once
#include <map>
#include <list>
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../class/common/Vector2.h"
#include "../class/common/ColorID.h"
#include "../class/common/Collision.h"

using MapData = std::map<std::string, std::vector<int>>;				// map<レイヤー名, 格納配列>
using AnimData = std::pair<int, int>;									// pair<画像内の何番目か, フレーム数>
using AnimVecData = std::list<AnimData>;								// AnimData を vectorで格納したデータ

// アイテムデータ格納(xml)
struct XmlItem
{
	std::map<std::string, std::string> item_;							// map<アイテム名, 要素>
	std::map<std::string, int> loop_;									// map<アニメーション名, ループ有無>
	std::map<std::string, AnimVecData> data_;							// map<アニメーション名, アニメーションデータ>
};

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string fileName);									// TSXの読み込み
	bool LoadTmx(std::string fileName);									// TMXの読み込み
	XmlItem LoadXmlItem(std::string fileName);							// XMLを読み込んでitemだけ返す
	bool SetMap(void);													// 読み込んだマップをmapdata_に格納する

	// Get関数
	const MapData& GetMapData(void)const { return mapdata_; };			// Mapdataの取得
	const VecInt& GetMapData(std::string lay);							// Mapdataの取得(layerdataから指定)
	const int GetMapData(std::string lay, int x, int y);				// mapdataの中身の取得(mapdata内の指定した位置)
	const int GetMapData(std::string lay, Vector2F pos);				// mapdataの中身の取得(posからmapdataの位置を計算)
	const bool GetMapDataCheck(Vector2F pos);							// blockの有無の取得(posからmapdataの位置を計算)
	const Vector2& GetWorldArea(void)const { return worldArea_; };		// チップ全体のサイズの取得
	const Vector2& GetTileSize(void)const { return tileSize_; };		// チップ1つのサイズの取得
	const unsigned int GetFirstGid(void)const { return firstGID_; };	// マップチップの開始番号の取得
	const unsigned int GetLayerSize(void)const { return layerSize_; };	// レイヤー数の取得
	const ColList& GetColList(void);									// コリジョンリストの取得

	const void ClearMapData(void);										// マップデータをクリア
private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);					// バージョンの確認
	std::map<std::string, int> version_;								// バージョンの対応有無

	// TMX
	rapidxml::xml_document<> tmxDoc_;									// TMXのドキュメント
	std::map<std::string, std::string> tmxitem_;						// attributeを取り出した後格納する変数
	unsigned int firstGID_;												// マップチップの開始番号
	unsigned int layerSize_;											// レイヤー数
	Vector2 worldArea_;													// レイヤーのチップ全体のサイズ
	Vector2 tileSize_;													// チップ1つのサイズ
	MapData mapdata_;													// マップデータの格納先
	ColList collist_;													// コリジョンの格納リスト

	// TSX
	rapidxml::xml_document<> tsxDoc_;									// TSXのドキュメント
	std::map<std::string, std::string> tsxitem_;						// attributeを取り出した後格納する変数

	// アイテム
	XmlItem xml_;
};

