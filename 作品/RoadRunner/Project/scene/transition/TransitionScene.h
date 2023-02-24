#pragma once
#include "../BaseScene.h"


enum class Transition
{
	CrossOver,
	//FadeInOut,
	CrossInOut,
	MAX,
};

class TransitionScene :
	public BaseScene
{
public:
	TransitionScene(uniqueBaseScene beforScene, uniqueBaseScene afterScene);
	~TransitionScene();
private:
	virtual bool UpdateTransition(void) = 0;

	uniqueBaseScene Update(uniqueBaseScene own) override;
	SceneID GetSceneID() override;
	bool Init() override;
protected:
	uniqueBaseScene beforScene_;					// 前シーン
	uniqueBaseScene afterScene_;					// 後シーン
};

