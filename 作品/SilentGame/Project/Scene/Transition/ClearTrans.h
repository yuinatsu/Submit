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
    // コンストラクタ 
    ClearTrans(std::shared_ptr<ObjManager> objMng, DrawMng& drawMng, Camera& camera, std::shared_ptr<TmxObj> tmxObj, std::shared_ptr<GmkEvent> gEvent, uniqueScene beforScene, uniqueScene afterScene);
    ~ClearTrans();
private:
    // トランジション更新 
    bool UpdataTransition(double delta);

    // 画面ズームの更新 
    void UpdateZoom(double delta);

    // 斬撃アニメーションの更新 
    void UpdateSlash(double delta);

    // 爆破アニメーションの更新 
    void UpdateExplosion(double delta);

    // トランジション描画 
    void DrawOwnScreen(float delta) override;

    // 画面ズームの描画 
    void DrawZoom(double delta);

    // 斬撃アニメーションの描画 
    void DrawSlash(double delta);

    // 爆破アニメーションの描画 
    void DrawExplosion(double delta);

    // 更新用のファンクション 
    std::function<void(double)> updateFunc_;

    // 描画用のファンクション 
    std::function<void(double)> drawFunc_;

    // トランジション完了フラグ 
    bool isEnd_;

    // 最大時間 
    double limitTime_;

    // 描画Manager 
    DrawMng& drawmng_;

    // 画面ズーム倍率
    double zoomExt_;

    // ズーム座標
    Math::Vector2 zoomPos_;

    // カメラ情報
    Camera& camera_;

    // ゲームシーンの最後のスクリーン情報
    int viewID_;

    // ステージ情報
    std::shared_ptr<TmxObj> tmxObj_;

    // ギミックイベント
    std::shared_ptr<GmkEvent> gEvent_;

    // エフェクトフラグ
    bool effectF_;

    // ズーム距離
    Math::Vector2 distance_;
};

