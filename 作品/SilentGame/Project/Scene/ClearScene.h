#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../common/Camera/Camera.h"

class ObjManager;
class UIMng;
class DrawMng;
class GmkEvent;
class TmxObj;
class ResultData;

class ClearScene :
    public BaseScene
{
public:
    ClearScene(int mapNum, bool isSmallMap);
    ~ClearScene();
private:
    bool Init(void) override;

    /// <summary> スクリーンや描画周りの初期化 </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool InitScreen(void);

    /// <summary> ゲームにかかわる初期化 </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool InitGame(void);

    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;                   // 自分自身を描画
    Scene GetSceneID(void) override { return Scene::Clear; };    // 自分はゲームシーン
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> 開始時の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    //uniqueScene UpdateStart(float delta, uniqueScene ownScene);

    /// <summary> ゲーム中の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene GameUpdate(float delta, uniqueScene ownScene);

    uniqueScene UpdateGameEnd(float delta, uniqueScene ownScene);

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void DrawMap(void);

    /// <summary> ロード完了時の処理 </summary>
    /// <param name=""></param>
    void Loaded(void) final;

    // オブジェクトを管理するクラス
    std::shared_ptr<ObjManager> objMng_;

    // UI管理クラス
    std::shared_ptr<UIMng> uiMng_;

    // カメラクラス
    Camera camera_;

    // ギミックイベントに関するクラス
    std::shared_ptr<GmkEvent> gEvent_;
    // 描画順番を管理し描画するクラス
    std::unique_ptr< DrawMng> drawMng_;

    // マップをロードするため
    std::string map_;
    std::shared_ptr<TmxObj> tmxObj_;

    // マップ上の描画用スクリーン
    int viewID_;

    // マップの床のスクリーン
    int mapFloor_;

    // アップデート用function
    std::function<uniqueScene(float, uniqueScene)> updateFunc_;

    std::function<void(float)> drawFunc_;

    // ゲームが開始されたかどうか
    //bool startedFlag_;

    int blendPic_;

    std::shared_ptr<ResultData> resultData_;

    // 画面ズーム倍率
    double zoomExt_;

    Math::Vector2 zoomPos_;

    bool isSmallMap_;
};

