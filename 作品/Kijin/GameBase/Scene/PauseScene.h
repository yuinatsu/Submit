#pragma once
#include "BaseScene.h"

class PauseScene :
    public BaseScene
{
public:
    PauseScene();
private:
    const SceneID GetID(void) const
    {
        return SceneID::Pause;
    }
    SceneUptr MakeGameFunc(SceneUptr own);
    void Update(float delta, Controller& controller) final;
    void DrawScene(void) final;
    // ���j���[UI�̏��������܂Ƃ߂�
    void Load(void);

    // �}�E�X�̍��W
    Vector2I mousePos_;
};

