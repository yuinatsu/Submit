#pragma once
#include "TransitionScene.h"
class FadeTrans :
    public TransitionScene
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="limitTime"> トランジション実行時間 </param>
    /// <param name="beforScene"> 前シーン </param>
    /// <param name="afterScene"> 後シーン </param>
    FadeTrans(double limitTime, uniqueScene beforScene, uniqueScene afterScene);
    ~FadeTrans();
private:
    /// <summary> トランジション更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <returns> トランジション完了:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> トランジション描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawOwnScreen(float delta) override;

    // <summary> 最大時間 </summary>
    double limitTime_;
};

