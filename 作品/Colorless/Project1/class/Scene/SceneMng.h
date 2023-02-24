#pragma once
#include <array>
#include <memory>
#include <chrono>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void) 
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);											// メインループ
	void Update(void);										// 更新
	void Draw(void);										// 描画
	const Vector2 GetScreenSize(void)const;					// 画面サイズ取得
	Stage GetStage(void);									// ステージ情報取得
	void SetStage(int num);									// ステージ情報セット
	char keyBuf_[256];										// 現在のキーバッファ
	char keyBufOld_[256];									// １つ前のキーバッファ
	CntType GetCntType(void);
	void SetCntType(CntType cntType);
private:
	SceneMng();
	~SceneMng();
	bool SysInit(void);										// システムの初期化
	bool initFlag_;											// 初期化用フラグ
	uniqueBaseScene scene_;									// シーンの格納先
	const Vector2 screenSize_;								// スクリーンサイズ
	Stage stage_;											// ステージ情報格納用
	CntType cntType_;
};