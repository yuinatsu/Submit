#include <DxLib.h>
#include "MapCtl.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// MapCtlの生成
//---------------
MapCtl::MapCtl()
{
}

// MapCtlの破棄
//---------------
MapCtl::~MapCtl()
{
}

// MapCtlの更新
//---------------
void MapCtl::Update(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)														// マップの縦のブロック数
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)													// マップの横のブロック数
		{
			if ((mapData[y][x] > 0) && (mapData[y][x] < BLOCK_MAX_SIZE_CNT))						// mapData[y][x]が０以上BLOCK_MAX_SIZE_CNT未満だった場合
			{
				mapData[y][x]++;																	// 加算
			}
		}
	}
}

// MapCtlの描画
//---------------
void MapCtl::Draw(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)														// マップの縦のブロック数
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)													// マップの横のブロック数
		{
			if (mapData[y][x] > 0)																	// mapDataが0より大きければ表示
			{
				DrawGraph(DRAW_OFFSET_X + x * CHIP_SIZE, DRAW_OFFSET_Y + y * CHIP_SIZE, blockImage[(mapData[y][x] / BLOCK_CNT_INV) % BLOCK_PTN_MAX], true);					// ブロック			// mapData[y][x]の数値が0,0,0,1,1,1,2,2,2......6,6,6まで行くようにする
			}
		}
	}
}

// MapCtlの初期化
//---------------
bool MapCtl::Init(void)
{
	bool rtnFlag = true;
	if (LoadDivGraph("Resource/image/block.png", BLOCK_PTN_MAX * 1, BLOCK_PTN_MAX, 1, CHIP_SIZE, CHIP_SIZE, blockImage) == -1)					// ブロック
	{
		rtnFlag = false;
	}

	memset(mapData, 0, sizeof(mapData));					// mapDataをセット

	return rtnFlag;
}

// MapCtlの解放
//---------------
bool MapCtl::Relese(void)
{
	for (int k = 0; k < BLOCK_PTN_MAX; k++)					// ブロックの解放
	{
		DeleteGraph(blockImage[k]);
	}
	return true;
}

// ブロックの配置
//-----------------
int MapCtl::SetBlock(Vector2 pos, int cnt)
{
	Vector2 chipPos;								// マス目変換
	chipPos.x = pos.x / CHIP_SIZE;					// プレイヤーのいる座標のブロックのマス目数
	chipPos.y = pos.y / CHIP_SIZE;					// プレイヤーのいる座標のブロックのマス目数

	if ((chipPos.x >= MAP_CHIP_COUNT_X) || (chipPos.x < 0) || (chipPos.y >= MAP_CHIP_COUNT_Y) || (chipPos.y < 0))			// マップの範囲チェック
	{
		return 0;					// マップ外に行ったら０を返す (マップ内であればif文を抜けて下に行く)
	}

	if (mapData[chipPos.y][chipPos.x] == 0)				// 指定マスへの情報の書き込み
	{
		mapData[chipPos.y][chipPos.x] = cnt;
	}

	return mapData[chipPos.y][chipPos.x];			// マップ内のchipの座標を返す
}

// マップ範囲チェック
//---------------------
int MapCtl::GetBlock(Vector2 pos)
{
	Vector2 chipPos;								// マス目変換
	chipPos.x = pos.x / CHIP_SIZE;					// プレイヤーのいる座標のブロックのマス目数
	chipPos.y = pos.y / CHIP_SIZE;					// プレイヤーのいる座標のブロックのマス目数

	if ((chipPos.x >= MAP_CHIP_COUNT_X) || (chipPos.x < 0) || (chipPos.y >= MAP_CHIP_COUNT_Y) || (chipPos.y < 0))			// マップの範囲チェック
	{
		//TRACE("範囲外\n");
		return BLOCK_MAX_SIZE_CNT;										// マップ外に行ったらブロックの最大サイズの値を返す (マップ内であればif文を抜けて下に行く)
	}

	return mapData[chipPos.y][chipPos.x];			// マップ内のchipの座標を返す
}

// ブロックサイズチェック
//-------------------------
bool MapCtl::isOnBlock(Vector2 pos)
{
	GetBlock(pos);																// GetBlock(pos)を呼び出し
	if (GetBlock(pos) >= BLOCK_MAX_SIZE_CNT)									// ブロックサイズが最大値だったら、trueを返す
	{
		return true;
	}
	return false;
}
