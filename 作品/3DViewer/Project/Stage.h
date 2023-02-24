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

	// ���f���̃n���h��ID
	int modelID_;

	// ���f���̕\�����W
	VECTOR pos_;
};

