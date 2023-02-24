#pragma once
#include <list>
#include <memory>
#include <vector>
#include <functional>
#include "../BaseScene.h"
#include "../../Object/Obj.h"
#include "../../common/Math.h"
class ObjManager;

/// <summary> トランジションの種類 </summary>
enum class Transition
{
    Fade,           // フェードインアウト
    Circle,         // サークル状にトランジション
    Clear,          // クリア時演出ありのフェードインアウト
    Dead,           // ゲームオーバー時演出ありのフェードインアウト
    Max
};

// 画面遷移用クラス
class TransitionScene :
    public BaseScene
{
public:
    /// <summary> コンストラクタ </summary>
    /// <param name="beforScene"> 移行前シーン </param>
    /// <param name="afterScene"> 移行後シーン </param>
    TransitionScene(uniqueScene beforScene, uniqueScene afterScene);
    ~TransitionScene();
private:
    /// <summary> 初期化 </summary>
    /// <returns> 正常動作:true </returns>
    bool Init(void);

    /// <summary> 更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <param name="ownScene"> 現在のシーン </param>
    /// <returns> 現在のシーン </returns>
    uniqueScene Update(float delta, uniqueScene ownScene) override;

    /// <summary> 描画 </summary>
    void Draw(void);

    /// <summary> トランジション更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    /// <returns> トランジション完了:true </returns>
    virtual bool UpdataTransition(double delta) = 0;
protected:
    /// <summary> シーンID取得 </summary>
    /// <returns> SceneID </returns>
    Scene GetSceneID(void) override final { return Scene::Transtion; };

    /// <summary> 前シーン </summary>
    uniqueScene beforScene_;

    /// <summary> 後シーン </summary>
    uniqueScene afterScene_;

    /// <summary> シーン移行フラグ取得 </summary>
    /// <returns> シーン移行フラグ </returns>
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> カメラサイズ </summary>
    Math::Vector2 cameraSize_;

    /// <summary> 画面オフセットサイズ </summary>
    Math::Vector2 offsetSize_;

    /// <summary> オフセット </summary>
    Math::Vector2 offset_;

    /// <summary> ObjクラスのManager </summary>
    std::shared_ptr<ObjManager> objMng_;

    /// <summary> フェードアウト時等のカウンタ </summary>
    double count;
};

