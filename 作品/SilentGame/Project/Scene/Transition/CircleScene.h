#pragma once
#include "TransitionScene.h"
class CircleScene :
    public TransitionScene
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="limitTime"> 最大時間 </param>
    /// <param name="beforScene"> 前シーン </param>
    /// <param name="afterScene"> 後シーン </param>
    CircleScene(double limitTime, uniqueScene beforScene, uniqueScene afterScene);
    ~CircleScene();
private:
    /// <summary> トランジション更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <returns> トランジション完了:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> トランジション描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawOwnScreen(float delta) override;

    /// <summary></summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="r"></param>
    /// <param name="Color"></param>
    /// <returns></returns>
    int DrawReversalCircle(int x, int y, int r, int Color);

    /// <summary> 最大時間 </summary>
    double limitTime_;
};

