#pragma once
#include "SCN_ID.h"
#include "MapCtl.h"
#include "Player.h"
#include "common.h"

#define CALL_IMAGE_SIZE_X 300							// 合図ロゴ横サイズ
#define CALL_IMAGE_SIZE_Y (240 / CALL_MAX)				// 合図ロゴ縦サイズ
#define CALL_MAX 3										// 合図ロゴ総数
#define START_CALL_MAX 2								// 開始合図ロゴ総数
#define CALL_COUNT_MAX 100								// 合図間隔

class GameScene
{
public:
	GameScene();												// GameSceneの生成
	~GameScene();												// GameSceneの破棄
	SCN_ID Update(char* keyData, char* keyDataOld);				// GameSceneの更新
	void Draw(void);											// GameSceneの描画
	void StartCallDraw(void);									// ゲーム開始前のカウントダウン表示
	bool Init(CHAR_ID char1, CHAR_ID char2);					// GaneSceneの初期化							// Init(1PのキャラID, 2PのキャラID)
	bool Relese(void);											// GameSceneの解放

	Player player[PLAYER_MAX];								// プレイヤー					// player[プレイヤー総数]
private:
	MapCtl mapCtl;											// ブロックの情報管理
	int bgImage_;											// ゲーム背景
	int callImage[CALL_MAX];								// コールロゴ					// callImage[合図ロゴ総数]
	bool deadFlag_ = false;									// 死亡フラグ
	bool callFlag_ = true;
	int callCnt_;											// 合図カウンタ
	Vector2 screenSize_;									// スクリーンサイズ
};

