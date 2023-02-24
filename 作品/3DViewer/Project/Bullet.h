#pragma once
#include "DxLib.h"
class SceneManager;

static constexpr float BULLET_SPEED = 5.0f;
static constexpr float BULLET_RAD = 50.0f;

class Bullet
{
public:
	Bullet(SceneManager* sceneManager);
	void Init(VECTOR pos, VECTOR angle);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	SceneManager* mSceneMnager_;

	// Pos
	VECTOR pos_;

	// ”ò‚ñ‚Å‚­Šp“x
	VECTOR angles_;
};

