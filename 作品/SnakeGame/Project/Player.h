#pragma once
#include "MapCtl.h"
#include "CharImage.h"
#include "Vector2.h"
#include "common.h"

#define PLAYER_MAX 2					// プレイヤー総数
#define PLAYER_DEF_SPEED 2              // プレイヤー移動量

class MapCtl;							// MapCtlのクラスのプロトタイプ宣言				メリット：ヘッダを読み込むよりコンパイル時間短縮、ヘッダの循環参照を防止

// キャラの状態
//---------------
enum class PL_ST
{
	NON,			// キャラ選択中
	ALIVE,			// 生存
	DEAD,			// 死亡
	MAX
};

class Player
{
public:
	Player();																		// Playerの生成
	~Player();																		// Playerの破棄
	PL_ST Update(MapCtl& mapCtl, char* keyData, char* keyDataOld);					// Playerの更新				// GetKeyState(マップデータ, 現在のキーデータ, 1フレーム前のキーデータ)
	void GetKeyState(char* keyData);												// キーデータ取得			// GetkeyState(キーデータ)
	void Draw(void);																// Playerの描画
	bool Init(CHAR_ID charID);														// Playerの初期化			// Init(選択キャラID)
	bool Relese(void);																// Playerの解放

	CHAR_ID charID_;				// キャラの種類別
	PL_ST state_;					// キャラの状態
	Vector2 pos_;					// 座標

	int keyNow_[static_cast<int>(DIR::MAX)];			// 入力状態のキー
	int keyOld_[static_cast<int>(DIR::MAX)];			// 入力状態のキー(1フレーム前)
private:
	static int pCnt;				// プレイヤーの総数
	int id_;						// プレイヤーID		1P:0, 2P:1
	DIR dir_;						// キャラの向き
	int speed_;						// 移動量
	CharImage charImage_;			// キャラ画像
	Vector2 IconPos_;				// キャラアイコンの座標
	int pAnimCnt_;					// プレイヤーアニメーションカウンタ

	int keyList_[static_cast<int>(DIR::MAX)];			// 各方向の対応キーリスト
};

