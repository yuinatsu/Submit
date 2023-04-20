#pragma once
#include "TransitionScene.h"
#include "../../common/DrawMng.h"
#include "../../common/Camera/Camera.h"
class DeadTrans :
    public TransitionScene
{
public:
    // コンストラクタ 
    DeadTrans(std::shared_ptr<ObjManager> objMng, Math::Vector2 offset, DrawMng& drawMng, Camera& camera, uniqueScene beforScene, uniqueScene afterScene);
    ~DeadTrans();
private:
    // トランジション更新 
    bool UpdataTransition(double delta);

    // トランジション描画 
    void DrawOwnScreen(float delta) override;

    // 最大時間 
    double limitTime_;

    // 描画Manager 
    DrawMng& drawmng_;

    // カメラ情報
    Camera& camera_;

    // ゲームシーンの最後のスクリーン情報
    int viewID_;
};

