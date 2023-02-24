#pragma once
#include "BaseScene.h"

class ObjectManager;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
private:
	const SceneID GetID(void) const
	{
		return SceneID::Title;
	}
	SceneUptr MakeGameFunc(SceneUptr own);
	void Update(float delta, Controller& controller) final;
	void DrawScene(void) final;
	// �摜���[�h
	SharedGraphicHandle logo_;
	SharedGraphicHandle start_;
};

