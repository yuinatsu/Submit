#pragma once
#include "TitleScene.h"
#include "RuleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "SCN_ID.h"

#define SCREEN_SIZE_X 800		// 画面の横サイズ
#define SCREEN_SIZE_Y 600		// 画面の縦サイズ

class SceneCtl
{
public:
	SceneCtl();								// SceneCtlの生成
	~SceneCtl();							// SceneCtlの破棄
	void Run(void);							// GAME MAIN
private:
	bool Init(void);						// SceneCtlの初期化
	bool SysInit(void);						// システム系初期化
	bool Relese(void);						// SceneCtlの解放
	void Update(void);						// SceneCtlの更新
	void Draw(void);						// SceneCtlの描画

	void GetKeyData(void);					// キーデータ取得

	void SetupScene(void);					// シーンのセットアップ
	void ReleseScene(void);					// 前シーンの解放

	TitleScene titleScene_;			// TitleSceneの情報管理
	RuleScene ruleScene_;			// RuleSceneの情報管理
	SelectScene selectScene_;		// SelectSceneの情報管理
	GameScene gameScene_;			// GameSceneの情報管理
	ResultScene resultScene_;		// ResultSceneの情報管理

	SCN_ID scnID_;					// 現在のSCN_ID
	SCN_ID oldScnID_;				// １つ前のSCN_ID
	SCN_ID nextScnID_;				// １つ後のSCN_ID
	char keyBuf_[256];				// 現在のキーバッファ
	char keyBufOld_[256];			// １つ前のキーバッファ
};

