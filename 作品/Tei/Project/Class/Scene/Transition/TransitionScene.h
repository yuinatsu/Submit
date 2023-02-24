#pragma once
#include "../BaseScene.h"

enum class Transition
{
    CrossFade,
    FadeInOut,
    Slide,
    Max,
};

class TransitionScene :
    public BaseScene
{
public:
    TransitionScene(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene);
    virtual ~TransitionScene();
private:
    virtual bool UpdateTransition(const double& delta) = 0;
    bool Init(void)override;
    const SceneID GetSceneID(void)const override final;
    std::unique_ptr<BaseScene> Update(const double& delta, std::unique_ptr<BaseScene> ownScene)override final;
protected:
    std::unique_ptr<BaseScene> beforScene_;
    std::unique_ptr<BaseScene> afterScene_;

    double count_;
};

