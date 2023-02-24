#pragma once
#include "SCN_ID.h"
#include "Player.h"
#include "CharImage.h"
#include "common.h"
#include "Vector2.h"

#define FINISH_COUNT_MAX 100					// 終わり合図のカウント

class ResultScene
{
public:
	ResultScene();														// ResultSceneの生成
	~ResultScene();														// ResultSceneの破棄
	SCN_ID Update(char* keyData, char* keyDataOld);						// ResultSceneの更新
	void Draw(void);													// ResultSceneの描画
	bool Init(CHAR_ID winnerID);										// ResultSceneの初期化				// Init(勝利したキャラのID)
	bool Relese(void);													// ResultSceneの解放
private:
	Player player[PLAYER_MAX];									// プレイヤー[プレイヤー総人数]
	CharImage charImage_;										// キャライメージ
	CHAR_ID winnerID_;											// 勝利したキャラのID
	Vector2 screenSize_;										// スクリーンサイズ
	Vector2 picPos_;											// キャラ立ち絵の座標
	Vector2 spacePos_;											// "PUSH SPACE KEY"の座標
	int bgImage_;												// ゲーム背景
	int winMesImage[static_cast<int>(CHAR_ID::MAX)];			// 勝利者メッセージロゴ[勝利したキャラの種類]
	int SpaceImage_;											// "PUSH SPACE KEY"
	int finishCnt_;												// 合図カウンタ
	int blinkCnt_ = 0;											// 点滅カウンタ
};

