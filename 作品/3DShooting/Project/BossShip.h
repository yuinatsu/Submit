#pragma once
#include <vector>
#include "Transform.h"

class SceneManager;
class Player;
class EventShot;
class Turret;

class BossShip
{
public:
	enum class BOSS_STATE
	{
		NON,
		EVENT,
		BATTLE,
		EXP,
		END,
	};
	BossShip(SceneManager* manager, Player* player);
	void Init(void);
	void Update(void);
	void UpdateTurret(void);
	void Draw(void);
	void DrawTurret(void);
	void Release(void);

	std::vector<Turret*> GetTurret(void);

	const int& GetModelID(void);
private:
	SceneManager* sceneManager_;
	Player* player_;
	EventShot* eventShot_;

	Transform transform_;

	float stepEvent_;

	std::vector<Turret*> turretList_;

	void MakeTurret(VECTOR localPos,VECTOR localAngle);
	void ExpInit(void);

	BOSS_STATE state_;

	std::vector<VECTOR> expPosList_;

	void ChengeState(BOSS_STATE state);
};

