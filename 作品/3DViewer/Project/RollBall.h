#pragma once
#include <dxlib.h>
#include "SceneManager.h"
class Unit;
class RollBall
{
public:
	RollBall(SceneManager* manager, Unit* unit);
	void Init();
	void Update();
	void Draw();
	void Release(void);

	VECTOR GetPos(void);
private:
	SceneManager* sceneMng_;
	Unit* unit_;

	VECTOR pos_;

	float step_;			// delta‚ÌŒo‰ßŽžŠÔ‰ÁŽZ
};

