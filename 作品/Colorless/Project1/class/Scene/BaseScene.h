#pragma once
#include <array>
#include <memory>
#include "../common/Vector2.h"
#include "../common/ColorID.h"

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;

// シーンの種類
enum class Scene
{
	Transition,				// シーン移行
	Title,					// タイトル
	Select,					// ステージセレクト
	Game,					// ゲーム実行
	Clear,					// クリア画面
	Max
};

// ステージの種類
enum class Stage
{
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual bool Init(void) = 0;														// 初期化
	virtual uniqueBaseScene Update(double delta,uniqueBaseScene own) = 0;				// 更新 Update(deltaTime, 各シーン)
	virtual void Draw(double delta);													// 描画 Draw(deltaTime)
	virtual void DrawOwnScreen(double delta) = 0;										// 各シーンの描画 Draw(deltaTime)
	virtual Scene GetSceneID(void) = 0;													// 各シーンの取得
protected:
	int screenID_;																		// スクリーン格納用
	Vector2 screenSize_;																// スクリーンサイズ
};

