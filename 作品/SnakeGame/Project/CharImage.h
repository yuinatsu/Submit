#pragma once
#include "common.h"

#define CHAR_SIZE_X 32										// キャラサイズ横サイズ
#define CHAR_SIZE_Y 32										// キャラサイズ縦サイズ

#define CHAR_PIC_MAX 5										// キャラ数
#define CHAR_PIC_SIZE_X (1750 / CHAR_PIC_MAX)				// キャラ立ち絵の横サイズ
#define CHAR_PIC_SIZE_Y 500									// キャラ立ち絵の縦サイズ


// キャラの方向(画像の並び方に合わせる)
//---------------------------------------
enum class DIR
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
	MAX
};

// キャラの種類
//---------------
enum class CHAR_ID
{
	REIMU,								// 霊夢
	MARISA,								// 魔理沙
	CIRNO,								// チルノ
	SAKUYA,								// 咲夜
	YOUMU,								// 妖夢
	DRAW,
	MAX
};

class CharImage
{
public:
	CharImage();																	// CharImageの生成
	~CharImage();																	// CharImageの破棄
	bool Init(CHAR_ID charID);														// CharImageの初期化			// Init(選択キャラID)
	bool Release(void);

	int chipImage[static_cast<int>(DIR::MAX)][ANIM_IMAGE_MAX];						// キャラチップ			// chipImage[方向][アニメーション数]// [(unsigned int)DIR::MAX]と同義		()の中に型を入力することでその直後の関数のみをその瞬間だけ()の中の型になる
	int iconImage_;																	// キャラアイコン
	int picImage[1][CHAR_PIC_MAX];													// キャラ立ち絵			// picImage[][キャラ数]

private:
	CHAR_ID charID_;																// キャラID
};

