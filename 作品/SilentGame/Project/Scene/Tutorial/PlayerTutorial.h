#pragma once
#include <memory>
#include <functional>
#include <list>
#include "../../common/Camera/Camera.h"
#include "../TutorialScene.h"

enum class TUTORIAL_1
{
    player,
    target,
    operation,
    walk,
    descriprion,
    dashcrouch,
    end,
    max
};

class PlayerTutorial :
    public TutorialScene
{
public:
    PlayerTutorial(int mapNum);
    ~PlayerTutorial();

    bool UpdateTutorial(float delta) override;
    void DrawTutorial(float delta) override;
    void DrawSummary(float delta) override;
    void DrawDescription(float delta) override;
private:
};

