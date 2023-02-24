#pragma once
#include "Transform.h"

class SceneManager;
class Player;

class Stage
{
public:

	Stage(SceneManager* manager,Player* player);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const int& GetModelHadle(void);
private:
	void DrawGrid(void);

	SceneManager* mSceneManager;

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform mTransform;

	Player* player_;

	int stageModel_;
	VECTOR stagePos_;

	int enemyModel_;
	VECTOR enemyPos_;
};