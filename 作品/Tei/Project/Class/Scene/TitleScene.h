#pragma once
#include "BaseScene.h"
class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

private:

    bool Init(void)override final;

    const SceneID GetSceneID(void)const override final;

    std::unique_ptr<BaseScene> Update(const double& delta, std::unique_ptr<BaseScene> ownScene)override final;

    void DrawOwnScreen(const double& delta)override final;

    Vector2 screenSize_;
};

