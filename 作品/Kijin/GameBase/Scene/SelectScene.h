#pragma once
#include "BaseScene.h"
class SelectScene :
    public BaseScene
{
public:
	SelectScene();
private:
	const SceneID GetID(void) const
	{
		return SceneID::Select;
	}

	// 新しいゲームシーンへの生成関数
	SceneUptr MakeGameFunc(SceneUptr own);
	// 続きのゲームシーンへの生成関数
	//SceneUptr MakeReGameFunc(SceneUptr own);
	// 設定シーンへの生成関数
	SceneUptr MakeOptionFunc(SceneUptr own);
	// 更新処理
	void Update(float delta, Controller& controller) final;
	// シーンの描画
	void DrawScene(void) final;
};

