#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../Object/ObjManager.h"
#include "../common/Camera/Camera.h"

class UIMng;
class DrawMng;
class GmkEvent;
class TmxObj;
class ResultData;
class ClearScene;
using VecInt = std::vector<int>;

enum class PLAYER_TYPE
{
    playerOne,
    playerTwo,
    playerThree,
    Max,
};


class TutorialScene :
    public BaseScene
{
public:
    TutorialScene();
    ~TutorialScene();
    void SoundPlay(void) override;
protected:
    bool Init(void) override;
    /// <summary> スクリーンや描画周りの初期化 </summary>
/// <param name=""></param>
/// <returns></returns>
    virtual bool InitScreen(void);

    /// <summary> ゲームにかかわる初期化 </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool InitGame(void);

    void UpdateSwitching(float count, bool moveFlag = false, bool hitFlag = false);

    // オブジェクトを管理するクラス
    std::shared_ptr<ObjManager> objMng_;

    // UI管理クラス
    std::shared_ptr<UIMng> uiMng_;

    BoxItem boxItem_;

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

    bool moveFlag_;
    int TutorialType_;
    float count_;
    VecInt paddata_;
    VecInt keydata_;
    // ゲームが開始されたかどうか
    bool startedFlag_;

    int blendPic_;

    // ゲームの概要の説明が終わったか
    bool sumFlag_;
    // 矢印を出した時の処理用変数
    bool updownFalg_;
    float speed_;

    float nowRate_;
    Math::Vector2I ViewSize_;
    bool hitFlag_;

    bool isFirst_;
    friend ClearScene;	 // リザルトシーンに渡すゲームの結果のデータ
    std::shared_ptr<ResultData> resultData_;

private:

    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;                   // 自分自身を描画
    Scene GetSceneID(void) override { return Scene::Tutorial; };    // 自分はゲームシーン
    bool GetFlag(void) override { return SceneFlag_; };

    /// <summary> 開始時の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateStart(float delta, uniqueScene ownScene);

    /// <summary> ゲーム中の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene GameUpdate(float delta, uniqueScene ownScene);

    uniqueScene UpdateGameEnd(float delta, uniqueScene ownScene);

    virtual bool UpdateTutorial(float delta) = 0;
    virtual void DrawTutorial(float delta) = 0;
    virtual void DrawSummary(float delta) = 0;
    virtual void DrawDescription(float delta) = 0;

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void DrawMap(void);

    /// <summary> ロード完了時の処理 </summary>
    /// <param name=""></param>
    void Loaded(void) final;

    // アップデート用function
    std::function<uniqueScene(float, uniqueScene)> updateFunc_;

    std::function<void(float)> drawFunc_;

    // 画面ズーム倍率
    double zoomExt_;

    Math::Vector2 zoomPos_;

    // fogやアラートの色のエフェクト
    struct ScreenEffect
    {
        Math::Vector2 pos;      // fogを薄くする中心座標
        float radius;           // fogを薄くする範囲の半径
        float redValue;         // アラート時の赤色の割合(1～0の値)
    };

    // 定数バッファのハンドル
    int cbuffH_;

    // 定数バッファ書き換え用ポインター
    ScreenEffect* buff_;

    // ピクセルシェーダのハンドル
    int psH_;
};

