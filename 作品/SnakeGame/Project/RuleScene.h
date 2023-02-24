#pragma once
#include "SCN_ID.h"
#include "Vector2.h"
#include "common.h"

class RuleScene
{
public:
	RuleScene();												// RuleSceneの生成
	~RuleScene();												// RuleSceneの破棄
	SCN_ID Update(char* keyData, char* keyDataOld);				// RuleSceneの更新
	void Draw(void);											// RuleSceneの描画
	bool Init(void);											// RuleSceneの初期化
	bool Relese(void);											// RuleSceneの解放
private:
	Vector2 spacePos_;											// "PUSH SPACE KEY"の座標
	Vector2 screenSize_;										// スクリーンサイズ
	int SpaceImage_;											// "PUSH SPACE KEY"
	int blinkCnt_ = 0;											// 点滅カウンタ
};

