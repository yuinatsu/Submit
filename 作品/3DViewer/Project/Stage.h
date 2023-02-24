#pragma once
#include <dxlib.h>
class SceneManager;
class Stage
{
public:
	Stage(SceneManager* manager);
	void Init();
	void Update();
	void Draw();
	void Release();
private:
	SceneManager* sceneMng_;

	// モデルのハンドルID
	int modelID_;

	// モデルの表示座標
	VECTOR pos_;
};

