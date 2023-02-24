#pragma once
#include "Transform.h"
#include <map>
#include <functional>
#include <vector>

class SceneManager;
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class PlayerShot;

class Player
{
public:

	enum class PLAYER_STATE
	{
		NON,
		RUN,
		EXP,
		END
	};

	static constexpr float COLLISION_RADIUS = 20.0f;

	Player(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform* GetTransForm(void);

	void ProcessTurn(void);
	void Turn(float angle, VECTOR axis);

	void Dead(void);
	bool isAlive(void);
	bool isEnd(void);

	void Shot(void);
	const std::vector<PlayerShot*>& GetShots(void)const;


	SpeechBalloon* speechBalloon_;
	
private:
	void ChengeState(PLAYER_STATE state);

	SceneManager* mSceneManager;
	ParticleGenerator* parGene_;
	SpriteAnimator* spriteAnimator_;

	// ���f������̊�{���
	Transform mTransform;

	std::vector<PlayerShot*> shots_;
	float shotInterval_;

	PLAYER_STATE state_;

	bool isAlive_;
};

