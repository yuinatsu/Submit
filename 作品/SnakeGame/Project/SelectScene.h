#pragma once
#include "SCN_ID.h"
#include "CharImage.h"
#include "Player.h"
#include "Vector2.h"
#include "common.h"

#define CHAR_SEL_IMAGE_SIZE_X 400							// キャラセレクトロゴのX座標
#define CHAR_SEL_IMAGE_SIZE_Y 80							// キャラセレクトロゴのY座標

#define CHAR_OPE_IMAGE_SIZE_X 200							// 選択操作ロゴのX座標
#define CHAR_OPE_IMAGE_SIZE_Y 40							// 選択操作ロゴのY座標

#define CHAR_OPE_MAX 4										// 選択操作ロゴの最大数

class SelectScene
{
public:
	SelectScene();																					// SelestSceneの生成
	~SelectScene();																					// SelestSceneの破棄
	SCN_ID Update(char* keyData, char* keyDataOld);													// SelestSceneの更新
	bool SelectChar(Player* player, DIR dir, int count, CHAR_ID limID, CHAR_ID setID);				// キャラセレクトキー入力関数 			// (選択プレイヤー, キー入力方向, IDの変化量, IDの最大値, IDの最大値まで行った時の値)
	bool DesideChar(Player* player, DIR dir, PL_ST state);											// キャラ決定キー入力関数				// (選択プレイヤー, キー入力方向, プレイヤーの状態)
	void Draw(void);																				// SelestSceneの描画
	bool Init(void);
	bool Relese(void);

	Player player[PLAYER_MAX];																// プレイヤー[プレイヤー総人数]
private:
	Vector2 screenSize_;																	// スクリーンサイズ
	CharImage charImage[static_cast<int>(CHAR_ID::MAX)];									// 選択キャラ用イメージ[キャラ総数]
	int logoImage_;																			// キャラ選択中のロゴ
	int bgImage_;																			// ゲーム背景
};

