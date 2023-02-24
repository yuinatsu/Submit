#pragma once
#include "Player.h"
#include "Vector2.h"
#include "MapCtl.h"
#include "SCN_ID.h"
#include "common.h"

#define TITLE_IMAGE_SIZE_X 720							// タイトルロゴ横サイズ
#define TITLE_IMAGE_SIZE_Y 350							// タイトルロゴ縦サイズ

#define START_COUNT_MAX 60								// 点滅カウント時間

class TitleScene
{
public:
	TitleScene();												// TitleSceneの生成
	~TitleScene();												// TitleSceneの破棄
	SCN_ID Update(char* keyData, char* keyDataOld);				// TitleSceneの更新				// char   変数渡し      char*   ポインタ渡し　　　char&   参照渡し(実体が必要)...参照の配列は使えない
	void Draw(void);											// TitleSceneの描画
	bool Init(void);											// TitleSceneの初期化
	bool Relese(void);											// TitleSceneの解放
private:
	Vector2 screenSize_;								// スクリーンサイズ
	int blinkCnt_;										// 点滅カウンタ
	int TitleBgImage_;									// タイトル背景
	int TitleImage_;									// タイトルロゴ
	int SpaceImage_;									// "PUSH SPACE KEY"
	bool startFlag_ = false;							// 点滅スピードフラグ
	int startCnt_;										// 点滅スピードカウンタ
};

