#pragma once
#include "TransitionScene.h"

class Slide :
    public TransitionScene
{
public:
    Slide(std::unique_ptr<BaseScene> beforScene, std::unique_ptr<BaseScene> afterScene,const double& limitTime = 3.0);
    virtual ~Slide();
private:
    double limitTime_;

    int beforScreen_;
    int afterScreen_;

    bool Init(void)override final;
    bool UpdateTransition(const double& delta);

    void DrawOwnScreen(const double& delta)override final;
};

