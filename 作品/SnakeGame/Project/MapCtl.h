#pragma once
#include "common.h"
#include "Vector2.h"

#define BLOCK_PTN_MAX 7															// 空白を含むブロックのパターン数
#define BLOCK_CNT_INV 3															// ブロックのサイズアップするフレームインターバル
#define BLOCK_MAX_SIZE_CNT ((BLOCK_PTN_MAX - 1) * BLOCK_CNT_INV)				// ブロックが最大サイズになるときのカウント数

class MapCtl
{
public:
	MapCtl();															// MapCtlの生成
	~MapCtl();															// MapCtlの破棄
	void Update(void);													// MapCtlの更新
	void Draw(void);													// MapCtlの描画
	bool Init(void);													// MapCtlの初期化
	bool Relese(void);													// MapCtlの解放
	int SetBlock(Vector2 pos, int cnt = 1);								// ブロックの配置			// SetBlock(プレイヤー座標, カウント)
	int GetBlock(Vector2 pos);											// マップ範囲チェック		// GetBlock(プレイヤー座標)
	bool isOnBlock(Vector2 pos);										// ブロックサイズチェック	// isOnBlock(プレイヤー座標)
private:
	int blockImage[BLOCK_PTN_MAX];								// ブロック画像
	int mapData[MAP_CHIP_COUNT_Y][MAP_CHIP_COUNT_X];			// マップサイズ
};

