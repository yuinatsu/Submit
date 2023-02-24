#pragma once
#include "../TutorialScene.h"
enum class TUTORIAL_3
{
    box,
    itember,
    use,
    alert,
    alerthit,
    gmk,
    end,
    max
};
class DoorTutorial :
    public TutorialScene
{
public:
    DoorTutorial(int mapNum);
    ~DoorTutorial();
    bool UpdateTutorial(float delta) override;
    void DrawTutorial(float delta) override;
    void DrawSummary(float delta) override;
    void DrawSummary2(float delta);
    void DrawDescription(float delta) override;
private:
    bool boxFlag_ = true;
    bool AlertFlag_ = false;
};

