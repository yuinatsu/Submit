#pragma once
#include "TransitionScene.h"
#include "../../common/DrawMng.h"
#include "../../common/Camera/Camera.h"
class TmxObj;
class GmkEvent;

class ClearTrans :
    public TransitionScene
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="objMng"> ObjクラスのManager </param>
    /// <param name="offset"> オフセット </param>
    /// <param name="drawMng"> 描画Manager </param>
    /// <param name="beforScene"> 前シーン </param>
    /// <param name="afterScene"> 後シーン </param>
    ClearTrans(std::shared_ptr<ObjManager> objMng, DrawMng& drawMng, Camera& camera, std::shared_ptr<TmxObj> tmxObj, std::shared_ptr<GmkEvent> gEvent, uniqueScene beforScene, uniqueScene afterScene);
    ~ClearTrans();
private:
    /// <summary> トランジション更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <returns> トランジション完了:true </returns>
    bool UpdataTransition(double delta);

    /// <summary> 画面ズームの更新 </summary>
    void UpdateZoom(double delta);

    /// <summary> 斬撃アニメーションの更新 </summary>
    void UpdateSlash(double delta);

    /// <summary> 爆破アニメーションの更新 </summary>
    void UpdateExplosion(double delta);

    /// <summary> トランジション描画 </summary>
    /// <param name="delta"> デルタタイム </param>
    void DrawOwnScreen(float delta) override;

    /// <summary> 画面ズームの描画 </summary>
    void DrawZoom(double delta);

    /// <summary> 斬撃アニメーションの描画 </summary>
    void DrawSlash(double delta);

    /// <summary> 爆破アニメーションの描画 </summary>
    void DrawExplosion(double delta);

    /// <summary> 更新用のファンクション </summary>
    std::function<void(double)> updateFunc_;

    /// <summary> 描画用のファンクション </summary>
    std::function<void(double)> drawFunc_;

    /// <summary> トランジション完了フラグ </summary>
    bool isEnd_;

    /// <summary> デルタタイム </summary>
    //double delta_;

    /// <summary> 最大時間 </summary>
    double limitTime_;

    /// <summary> 描画Manager </summary>
    DrawMng& drawmng_;

    // 画面ズーム倍率
    double zoomExt_;

    Math::Vector2 zoomPos_;

    Camera& camera_;

    int viewID_;

    std::shared_ptr<TmxObj> tmxObj_;

    std::shared_ptr<GmkEvent> gEvent_;

    bool effectF_;

    Math::Vector2 distance_;
};

