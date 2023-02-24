#pragma once
#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
private:
	bool Init(void) override;												// 初期化
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// 更新 Update(deltaTime, 各シーン)
	void DrawOwnScreen(double delta);										// 各シーンの描画
	Scene GetSceneID(void) override { return Scene::Title; };				// 各シーンの取得
	Vector2F titlePos_;
	bool titleFlag_;
	double count_;
	bool logoFlag_;
	int titleBGM_;
	int titleSE_;
};

