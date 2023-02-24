#pragma once
#include "../TutorialScene.h"

enum class TUTORIAL_2
{
    itember,
    knife,
    box,
    item,
    use,
    end,
    max
};

class ItemTutorial :
    public TutorialScene
{
public:
    ItemTutorial(int mapNum);
    ~ItemTutorial();
    bool UpdateTutorial(float delta) override;
    void DrawTutorial(float delta) override;
    void DrawSummary(float delta) override;
    void DrawDescription(float delta) override;
private:
    bool EnemyFlag_ = false;
    //  どの箱が開けられたか
    bool box1_ = true;
    bool box2_ = true;
    bool box3_ = true;
    bool box4_ = true;
    // 各ボックスの座標
    int box1X_ = 292;
    int box2X_ = 356;
    int box3X_ = 420;
    int box4X_ = 484;
    int boxY_ = 100;
    // アイテム説明が出る座標
    int ItemPosX_ = 80;
    int ItemPosY_ = 200;
};

