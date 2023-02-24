#pragma once
#include "TransitionScene.h"
class CrossOver :
    public TransitionScene
{
public:
    CrossOver(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene, const double& limitTime = 3.0);
    virtual ~CrossOver()override final;
private:
    double limitTime_;

    bool UpdateTransition(const double& delta);
    void DrawOwnScreen(const double& delta)override final;
};

