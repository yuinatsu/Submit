#pragma once
#include <memory>
#include "../common/Vector2.h"

enum class SceneID
{
	Transition,
	Title,
	Game,
	Max,
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();

	virtual std::unique_ptr<BaseScene> Update(const double& delta, std::unique_ptr<BaseScene> ownScene) = 0;

	virtual void Draw(const double& delta);
	virtual void DrawOwnScreen(const double& delta) = 0;

	virtual bool Init(void) = 0;

	virtual const SceneID GetSceneID(void)const = 0;

private:

protected:

	int screenID_;
	Vector2 screenSize_;
};

