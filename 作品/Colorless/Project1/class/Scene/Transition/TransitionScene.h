#pragma once
#include "../BaseScene.h"

// Transitionの種類
enum class Transition 
{
	CrossFade,
	FadeInOut,
	Max
};

class TransitionScene :
	public BaseScene
{
public:
	TransitionScene(uniqueBaseScene beforScene,uniqueBaseScene afterScene);			// TransitionScene(変更前のシーン, 変更後のシーン)
	~TransitionScene();
private:
	bool Init(void) override;														// 初期化
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;				// 更新 Update(deltaTime, 各シーン情報)
	virtual bool UpdateTransition(double delta) = 0;								// Tansitionの更新 UpdateTransition(deltaTime)
protected:
	Scene GetSceneID(void) override final { return Scene::Transition; };
	uniqueBaseScene beforScene_;													// 変更前のシーン
	uniqueBaseScene afterScene_;													// 変更後のシーン
	double count_;																	// 1フレームあたりの値
};

