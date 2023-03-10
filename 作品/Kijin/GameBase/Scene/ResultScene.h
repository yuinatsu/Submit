#pragma once
#include "BaseScene.h"

class ResultScene :
    public BaseScene
{
public:
	ResultScene(ResultAttribute atr);
private:
	const SceneID GetID(void) const
	{
		return SceneID::Result;
	}
	SceneUptr MakeSelectFunc(SceneUptr own);
	SceneUptr MakeRetryFunc(SceneUptr own);
	// メニューUIの初期化をまとめる
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;
};
