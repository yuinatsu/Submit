#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "../Object/UI/UiBase.h"
#include "../Object/Obj.h"
#include "../Object/ObjManager.h"
class LoadScene :
    public BaseScene
{
public:
    LoadScene(double limitTime,uniqueScene before,uniqueScene after);
    ~LoadScene();
private:
    bool Init(void) override;
    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;
    Scene GetSceneID(void) override { return Scene::Load; };
    bool GetFlag(void) override { return SceneFlag_; };

    void LoadPic(void);
    void DrawLoadPic(double delta);

    uniqueScene before_;
    uniqueScene after_;

    // 時間系
    double time;    // 画像切り替え
    double count; 
    double limitTime_;

    int stringPic_;
    int stringCount_ = 0;
    int loadendStringPic_;
    int loadEndCount_ = 0;
    bool stringFlag = true;
    bool loadStringFlag = true;
    // ゲーム概要と敵の説明以外のストリングを画像に書き直そう

    // アイテム説明の画像
    static const int picCount = 3;
    int loadPic[picCount];
};

