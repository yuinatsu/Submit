#pragma once
#include <list>
#include <memory>
#include <functional>
#include "BaseScene.h"

class ViewerScene :
    public BaseScene
{
public:
    ViewerScene();
    ~ViewerScene();
private:
    // フィールドの格子線数
    static constexpr int rayNum = 30;
    // 移動量
    static constexpr float moveValue = 10;

    // ライト行列
    struct LightMatrix
    {
        MATRIX view;                                                // ビュー
        MATRIX projection;                                          // 射影
    };
    // カメラ情報
    struct Camera
    {
        static constexpr float ortPro = 2000.0f;                    // 正射影の表示範囲
        static constexpr float disNear = 0.001f;                    // 手前の距離
        static constexpr float disFar = 4000.0f;                    // 奥の距離
        Vector3 target;                                             // 注視点
        Vector3 pos;                                                // 座標
        Vector3 rot;                                                // 回転
        Vector3 angle;                                              // 角度
    };
    // モデル情報
    struct Model
    {
        int handle;                                                 // グラフィックハンドル
        Vector3 pos;                                                // 座標
        float angle;                                                // 角度
        Vector3 size;                                               // サイズ
        int mesh;                                                   // 影描画メッシュ
    };
    // ステージ情報
    struct Stage
    {
        int handle;                                                 // グラフィックハンドル
        Vector3 size;
        int mesh;                                                   // 影描画メッシュ
    };
    // ポストプロセス
    struct PostProcess
    {
        bool flag;                                                  // ポストエフェクトが有効か
        int tex;                                                    // オフスクリーン用テクスチャ
        std::array<int, 10> ps;                                     // ピクセルシェーダ
    };
    // ブラー
    struct Blur
    {
        int side;                                                   // 横ブラー
        int vert;                                                   // 縦ブラー
    };
    // 定数バッファ
    struct CBuffer
    {
        int ps;                                                     // ピクセル
        int vs;                                                     // 頂点
    };
    // シャドーマップ
    struct ShadowMap
    {
        int ps;                                                     // シャドーピクセル
        std::array<int, 6> vs;                                      // シャドー頂点
        int setPS;                                                  // ピクセルセット
        int tex;                                                    // 影テクスチャ
    };
    // ピクセル種類
    struct Pixel
    {
        int toon;                                                   // アニメ調
        int lam;                                                    // 拡散反射
        int tex;                                                    // テクスチャ
    };

    // 初期化
    bool Init(void) override;
    // カメラ情報初期化
    void InitCamera(void);
    // モデル関係初期化
    void InitModel(void);
    // ライト初期化
    void InitLight(void);
    // 影関係初期化
    void InitShadow(void);
    // ポストプロセス初期化
    void InitPostProcess(void);
    // トーンマップ初期化
    void InitToonMap(void);
    // シェーダー初期化
    void InitShader(int model);

    // 更新
    SceneUptr Update(SceneUptr ownScene, float delta) override;
    // 描画
    void DrawOwnScreen(float delta) override;
    // シーンID取得
    SceneID GetSceneID(void) override { return SceneID::Viewer; }
    // 解放
    void Relese(void);

    // モデル移動制御
    void ProcessMove(void);
    // モデル回転制御
    void ProcessRotate(void);

    // 影用の深度記録画像生成
    void CreateShadowMap(void);
    // モデル描画(シャドーマップ込み)
    void DrawOffScreen(void);

    // ポストプロセス実行
    void DrawPostProcess(Vector2 pos, int offScreen, int shader, bool flag);
    // 頂点情報格納
    VERTEX2DSHADER StorageVertex(VERTEX2DSHADER vert, Vector2 pos, Vector2 texPos);

    // 影表現用の変数
    LightMatrix* lightMat_;
    LightMatrix lightM_;
    // カメラ
    Camera camera_;
    // モデル
    Model model_;
    // ステージ
    Stage stage_;
    // シェーダ
    Pixel pixel_;           // ピクセルシェーダ
    int vs_;                // 頂点シェーダ
    int toonMap_;           // トゥーンマップ(光が当たってない面を暗くする)
    // シャドウマップの作成
    ShadowMap shadowMap_;
    // ポストエフェクト用
    PostProcess post_;
    // 被写界深度用
    Blur blur_;
    // 定数バッファ
    CBuffer cBuf_;

    // アニメーション
    int attachIndex_;
    float totalTime_;
    float playTime_;

    // 回転処理
    float rot_;         // 回転量
};

