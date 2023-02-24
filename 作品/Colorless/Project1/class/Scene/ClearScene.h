#pragma once
#include "BaseScene.h"
class ClearScene :
	public BaseScene
{
public:
	ClearScene();
	~ClearScene();
private:
	bool Init(void) override;												// 初期化
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// 更新 Update(deltaTime, 自身のシーン)
	void DrawOwnScreen(double delta);										// 各シーンの描画 DrawOwnScene(deltaTime)
	Scene GetSceneID(void) override { return Scene::Clear; };				// 各シーンの取得

	Vector2F cursorPos_;
	int cursorNum_;
	Vector2F shadowPos_;
	int blinkCnt_;
	int clearSE_;
};

