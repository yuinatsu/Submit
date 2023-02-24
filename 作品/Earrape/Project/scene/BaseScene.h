#pragma once
#include <memory>
#include <list>
#include "../common/SaveLoad.h"
#include "../common/DataID.h"
#include "../SCN_ID.h"

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;

constexpr int AtkPoint = 1000;										//武器の基礎攻撃力（仮）
constexpr int FontOffset = 120;										//フォントのオフセット（仮）
constexpr int LimitTimeDigit = 2;									//制限時間の桁数（記号は含まない)
constexpr int ScoreDigit = 7;										//スコアの桁数

//各フォントの大きさ定義
constexpr int FontNumSizeX = 60;									//数字のフォントサイズＸ
constexpr int FontNumSizeY = 60;									//数字のフォントサイズＹ
constexpr int TotalLimitTimeSizeX = FontNumSizeX * LimitTimeDigit;	//制限時間のサイズ合計Ｘ
constexpr int TotalScoreSizeX = FontNumSizeX * ScoreDigit;			//スコアのサイズ合計Ｘ
constexpr int EarSizeX = 205;										//耳の大きさＸ
constexpr int EarSizeY = 300;										//耳の大きさＹ 

class BaseScene
{
public:
	BaseScene() {};
	virtual ~BaseScene() {};
	virtual SCN_ID Update(char* keyData, char* keyDataOld) = 0;		// 更新(現在のキー情報, 過去のキー情報)
	virtual void Draw() = 0;										// 描画
	virtual bool Init(void) = 0;									// 初期化
	virtual bool Release(void) = 0;									// 解放
};
