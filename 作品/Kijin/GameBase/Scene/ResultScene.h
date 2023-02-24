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
	// ���j���[UI�̏��������܂Ƃ߂�
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;
};
