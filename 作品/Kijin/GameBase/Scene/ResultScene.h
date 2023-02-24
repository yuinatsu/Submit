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
	// ƒƒjƒ…[UI‚Ì‰Šú‰»‚ğ‚Ü‚Æ‚ß‚é
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;
};
