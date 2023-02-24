#pragma once
#include <list>
#include "BaseScene.h"

class MapData;
class Object;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();   
private:
    bool Init(void)override final;
    void AnimInit(void);

    const SceneID GetSceneID(void)const override final;

    std::unique_ptr<BaseScene> Update(const double& delta, std::unique_ptr<BaseScene> ownScene)override final;

    void DrawOwnScreen(const double& delta)override final;

    std::shared_ptr<MapData> mapData_;
    std::list<std::shared_ptr<Object>> objList_;
};

