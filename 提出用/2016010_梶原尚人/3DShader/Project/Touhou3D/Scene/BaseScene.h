#pragma once
#include <memory>
#include "../Common/Math.h"
#include"../Input/Controller.h"

class BaseScene;
using SceneUptr = std::unique_ptr<BaseScene>;

// シーンID
enum class SceneID
{
	Viewer,			// ビュアー
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化
	virtual bool Init(void) = 0;
	// 更新
	virtual SceneUptr Update(SceneUptr ownScene, float delta) = 0;
	// 描画
	virtual void Draw(void);
	// 現在のシーンの描画
	virtual void DrawOwnScreen(float delta) = 0;
	// 現在のシーンの取得
	virtual SceneID GetSceneID(void) = 0;
protected:
	// スクリーンサイズ
	Vector2I screenSize_;
	// スクリーンデータ
	int screenID_;
	// コントローラ情報
	std::unique_ptr<Controller> controller_;
};

