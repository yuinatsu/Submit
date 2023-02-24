#pragma once

#include <string>
#include <vector>
#include <map>
#include "../RapidXml/rapidxml.hpp"
#include "../../common/Vector2.h"


// データ格納構造体
struct Layer
{
	std::string name;
	std::string data;
};

// マップ作成時必要情報構造体
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
// そんな長くないけどusingしとくか感
using mapStr = std::vector<Layer>;

namespace Loader
{
	class TmxLoader
	{
	public:
		TmxLoader();
		TmxLoader(const char* filename);
		bool TmxLoad(std::string filename);			// Tmxファイルロード用 内部でTsxLoadを呼んでいる
		const mapStr GetmapStr(void);				// ロードしてきたstringを渡す奴
		const std::string GetMapKey(void);			// Tsxロード時作成されたImageへアクセスするためのキー
		const mapInfo GetMapInfo(void);				// マップ作成時に必要な情報セット
		const std::vector <colDataS>& GetColDataS(void);			// ColDataSの獲得
		~TmxLoader();
	private:
		void VersionMap(void);						// 対応バージョン格納するためだけの奴
		int  GetLayerSize(void);					// レイヤーの最大数獲得するだけの奴
		bool LoadColData(void);						// colision用のデータの読み込み
		bool TsxLoad(std::string filename);			// Tsxファイルをロード TmxLoadから呼び出される

		rapidxml::xml_document<> TmxDoc_;
		rapidxml::xml_document<> TsxDoc_;

		rapidxml::xml_node<>* tmxNode_;		// 一番最初のノード 基本的にここからアクセスする
		rapidxml::xml_node<>* tsx_orign_node_;

		std::map<std::string, int> version_;		// 今後別バージョンに対応した際こいつに対応したバージョンをキーにして適当な値を入れ入れとけば対応してるかどうか確認するときに楽
		mapStr mapStr_;								// 読み込んだstringの保存場所
		std::vector<colDataS> colDataSvec_;			// 読み込んだCollision用の情報保存先
		mapInfo info_;								// 読み込んだマップ作製用情報を使いやすい形に変え格納したもの
	};
}

