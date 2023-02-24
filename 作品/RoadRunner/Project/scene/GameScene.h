#pragma once
#include "BaseScene.h"
class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
private:
	uniqueBaseScene Update(uniqueBaseScene) override;
	SceneID GetSceneID() override;
	void DrawOwnScreen() override;
	bool Init(void) override;

	int gameScrID_;
};

