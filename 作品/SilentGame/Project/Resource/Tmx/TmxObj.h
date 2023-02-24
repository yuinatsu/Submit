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

// ドアのデータ(当たり判定のidxと中心座標)
using DoorData = std::pair<int, Math::Vector2>;
//using AnimData = std::pair<int, int>;									// pair<画像内の何番目か, フレーム数>
//using AnimVecData = std::list<AnimData>;								// AnimData をlistで格納したデータ

// xmlデータ格納
//struct XmlItem
//{
//	std::map<std::string, std::string> item_;							// map<アイテム名, 要素>
//	std::map<std::string, int> loop_;									// map<アニメーション名, ループ有無>
//	std::map<std::string, AnimVecData> data_;							// map<アニメーション名, アニメーションデータ>
//};

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTSX(std::string fileName);
	bool LoadTMX(std::string fileName);

	//XmlItem LoadXML(std::string fileName);


	// ワールド座標の取得
	const Math::Vector2I& GetWorldArea(void);

	// タイルサイズの取得
	const Math::Vector2I& GetTileSize(void);

	// レイヤーサイズの取得
	const unsigned int GetLayerSize(void);

	// GIDの取得
	const unsigned int GetFirstGID(void);

	// マップデータの取得
	const MapData& GetMapData(void);

	// posは座標としたマップデータを取得
	const int GetMapData(std::string layer, Math::Vector2I pos);

	// x,yはマス目数としたマップデータの取得
	const int GetMapData(std::string layer, int x, int y);

	// Collisionを取得
	ColData& GetColList(void);

	/// <summary> 指定の番号のコリジョンを無効にする </summary>
	/// <param name="idx"> 指定の場所 </param>
	/// <return> 指定の場所 </return>
	int OffCol(int idx);

	/// <summary> offにしたコリジョンを有効にする </summary>
	/// <param name="idx"> 追加する </param>
	/// <returns> 追加した後の番号 </returns>
	int OnCol(int idx);

	/// <summary> ドアのデータを取得する </summary>
	/// <param name=""> ドアのデータのリストの参照 </param>
	/// <returns></returns>
	std::list<DoorData>& GetDoorData(void);
	
	/// <summary> 経路探査 </summary>
	/// <param name="x"> xマス目 </param>
	/// <param name="y"> yマス目 </param>
	/// <returns> trueの時経路探査可能 </returns>
	const bool GetNaviFlag(int x, int y) const;

	/// <summary> 経路探査可能かのフラグをセットする </summary>
	/// <param name="x"> xマス目 </param>
	/// <param name="y"> yマス目 </param>
	/// <param name="flag"> フラグ </param>
	void SetNaviFlag(int x, int y, bool flag);

	/// <summary> 経路探査可能化のフラグをセットする </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="flag"> フラグ </param>
	void SetNaviFlag(const Math::Vector2& pos, bool flag);

	void SetNaviFlag(const Math::Vector2& pos, float r, bool flag);



private:

	/// <summary> マップのセット </summary>
	/// <param name=""></param>
	/// <returns> 成功時true失敗時false </returns>
	bool SetMap(void);

	bool SetCol(rapidxml::xml_node<>* mapNode);


	bool CheckTiledVersion(rapidxml::xml_node<>* node);


	std::map<std::string, int> version_;

	// TMX
	rapidxml::xml_document<> tmxDoc_;

	// 先頭のID
	unsigned int firstGID_;

	// レイヤー数
	unsigned int layerSize_;

	// マップのチップ数
	Math::Vector2I worldArea_;

	// タイルのサイズ
	Math::Vector2I tileSize_;

	// マップデータ
	MapData mapData_;


	ColData colList_;				// プレイヤーとエネミーどちらにも反応するコリジョンデータ
	
	ColData offColList_;			// 現在無効になっている当たり判定
	
	std::list<DoorData> doorList_;				// ドアのデータ



	// 現在のマス目が経路探査可能かを保持
	std::vector<std::vector<bool>> naviNodeFlag_;


	// TSX
	rapidxml::xml_document<> tsxDoc_;

};

