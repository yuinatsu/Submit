#pragma once
#include "TransitionScene.h"
#include "../../common/DrawMng.h"
#include "../../common/Camera/Camera.h"
class DeadTrans :
    public TransitionScene
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="objMng"> ObjクラスのManager </param>
    /// <param name="offset"> オフセット </param>
    /// <param name="drawMng"> 描画Manager </param>
    /// <param name="beforScene"> 前シーン </param>
    /// <param name="afterScene"> 後シーン </param>
    DeadTrans(std::shared_ptr<ObjManager> objMng, Math::Vector2 offset, DrawMng& drawMng, Camera& camera, uniqueScene beforScene, uniqueScene afterScene);
    ~DeadTrans();
private:
    /// <summary> トランジション更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <returns> トランジション完了:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> トランジション描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawOwnScreen(float delta) override;

    /// <summary> 最大時間 </summary>
    double limitTime_;

    /// <summary> 描画Manager </summary>
    DrawMng& drawmng_;

    int viewID_;

    Camera& camera_;
};

