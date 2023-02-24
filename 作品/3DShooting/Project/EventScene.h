#pragma once
#include "SceneBase.h"

class SceneManager;
class SpaceDome;
class BossShip;

class EventScene:
	public SceneBase
{
public:
	EventScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
private:
	SpaceDome* mSpaceDome;
	BossShip* boss_;
};

