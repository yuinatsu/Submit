#include <DxLib.h>
#include <array>
#include <cassert>
#include "ViewerScene.h"
#include "../Common/ImageMng.h"
#include "../Common/Debug.h"

ViewerScene::ViewerScene()
{
    // 初期化
    Init();
}

ViewerScene::~ViewerScene()
{
    // 解放
    Relese();
}

bool ViewerScene::Init(void)
{
    // カメラ初期化
    InitCamera();
    // モデル関係初期化
    InitModel();
    // ライト初期化
    InitLight();
    // 影関係初期化
    InitShadow();
    // ポストプロセス初期化
    InitPostProcess();
    // トーン用の画像をセット
    InitToonMap();
    // シェーダー初期化
    InitShader(model_.handle);

    fieldFlag_ = false;
    is2DOperation_ = false;

    return true;
}

void ViewerScene::InitCamera(void)
{
    // カメラ
    camera_.target = zeroVector3<float>;
    camera_.pos = zeroVector3<float>;
    camera_.rot = zeroVector3<float>;
    camera_.angle = zeroVector3<float>;
}

void ViewerScene::InitModel(void)
{
    // モデル
    model_.handle = MV1LoadModel("./Resource/Model/miku.mv1");
    model_.pos = zeroVector3<float>;
    model_.angle = 0.0f;
    model_.size = Vector3(15.0f, 15.0f, 15.0f);
    // ステージ
    stage_.handle = MV1LoadModel("./Resource/Model/Stage3.mv1");
}

void ViewerScene::InitLight(void)
{
    // デフォルトライト設定
    SetLightDirection(VGet(1.0f, -1.0f, 1.0f));                 // 方向
    SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));        // ディフューズカラー
    SetLightSpcColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));        // スペキュラカラー
    SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.0f));        // アンビエントカラー

    // ライト行列初期化
    lightM_.view = MGetIdent();
    lightM_.projection = MGetIdent();

    // 定数バッファ(シェーダー)
    cBuf_.vs = CreateShaderConstantBuffer(sizeof(LightMatrix) * 4);                    // ハンドル初期化
    lightMat_ = static_cast<LightMatrix*>(GetBufferShaderConstantBuffer(cBuf_.vs));    // 定数バッファのアドレス取得
}

void ViewerScene::InitShadow(void)
{
    shadowMap_.vs[0] = LoadVertexShader("Shader/ShadowMap/Mesh1Shadow.vso");
    shadowMap_.vs[1] = LoadVertexShader("Shader/ShadowMap/Mesh4Shadow.vso");
    shadowMap_.ps = LoadPixelShader("Shader/ShadowMap/ShadowMapPS.pso");
    shadowMap_.setPS = LoadPixelShader("Shader/Shadow/setShadowMap.pso");

    // ステージ
    stage_.mesh = LoadVertexShader("Shader/Shadow/stage2VS.vso");
    // モデル
    model_.mesh = LoadVertexShader("Shader/Shadow/model2VS.vso");

    // シャドーマップ生成
    SetDrawValidFloatTypeGraphCreateFlag(true);     // 浮動小数点型
    SetCreateDrawValidGraphChannelNum(1);           // 1チャンネル
    SetCreateGraphColorBitDepth(16);                // 16ビット
    shadowMap_.tex = MakeScreen(screenSize_.x, screenSize_.y, false);           // 画像生成
    // 設定を元に戻す
    SetDrawValidFloatTypeGraphCreateFlag(false);    // 整数型
    SetCreateDrawValidGraphChannelNum(4);           // 4チャンネル
    SetCreateGraphColorBitDepth(32);                // 32ビット
}

void ViewerScene::InitPostProcess()
{
    // ポストプロセス
    post_.flag = false;
    post_.tex = MakeScreen(screenSize_.x, screenSize_.y);                      // ポストプロセス用オフスクリーン
    post_.ps[0] = LoadPixelShader("Shader/PostEffect/aveblur.pso");            // ブラー
    post_.ps[1] = LoadPixelShader("Shader/PostEffect/mono.pso");               // モノトーンカラー
    // 被写界深度
    blur_.side = MakeScreen(screenSize_.x / 2, screenSize_.y, false);          // 横ブラー
    blur_.vert = MakeScreen(screenSize_.x / 2, screenSize_.y / 2, false);      // 縦ブラー
}

void ViewerScene::InitToonMap(void)
{
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    // トーンマップセット
    MV1SetPosition(model_.handle, VGet(model_.pos.x, model_.pos.y, model_.pos.z));
    MV1SetScale(model_.handle, VGet(model_.size.x, model_.size.y, model_.size.z));
    MV1SetPosition(stage_.handle, VGet(0.0f, 0.0f, 0.0f));
    MV1SetScale(stage_.handle, VGet(1.0f, 1.0f, 1.0f));
}

void ViewerScene::InitShader(int model)
{
    int vertType = -1;
    for (int i = 0; i < MV1GetTriangleListNum(model); ++i)
    {
        vertType = MV1GetTriangleListVertexType(model, i);        // モデルのトライアングルリストの頂点データタイプを取得
        break;
    }
    switch (vertType)
    {
    case DX_MV1_VERTEX_TYPE_1FRAME : 
        vs_ = LoadVertexShader("Shader/Vertex/Mesh.vso");
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        vs_ = LoadVertexShader("Shader/Vertex/Mesh4.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        vs_ = LoadVertexShader("Shader/Vertex/NormMesh.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        vs_ = LoadVertexShader("Shader/Vertex/NormMesh4.vso");
        break;
    default:
        break;
    }
    // 法線マップ用の情報が含まれているか
    if (vertType <= DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
    {
        pixel_.tex = LoadPixelShader("Shader/Pixel/tex.pso");
        pixel_.lam = LoadPixelShader("Shader/Pixel/Lambert.pso");
        pixel_.toon = LoadPixelShader("Shader/Pixel/Toon.pso");
    }
    else
    {
        pixel_.tex = LoadPixelShader("Shader/Pixel/NormTex.pso");
        pixel_.lam = LoadPixelShader("Shader/Pixel/NormLambert.pso");
        pixel_.toon = LoadPixelShader("Shader/Pixel/NormToon.pso");
    }

    // シェーダー設定
    SetUseVertexShader(vs_);                // 頂点シェーダー設定
    SetUsePixelShader(pixel_.tex);          // ピクセルシェーダー設定
    // トーンマップ
    SetDrawMode(DX_DRAWMODE_BILINEAR);      // トーンマップの画質劣化を防ぐためにバイリニア法で描画
    SetUseTextureToShader(3, toonMap_);     // シェーダー描画で使用するグラフィックハンドル設定
    // Zバッファ設定
    SetUseZBuffer3D(true);                  // 使用できるようにする
    SetWriteZBuffer3D(true);                // 書き込みを行えるようにする
}

SceneUptr ViewerScene::Update(SceneUptr ownScene, float delta)
{
    controller_->Update(delta);
    // 移動量
    auto move = moveValue;
    if (controller_->Press(InputID::Dash))          move *= 3;
    if (controller_->Press(InputID::Dimension))     is2DOperation_ = true;
    else is2DOperation_ = false;

    // モデル移動
    if (controller_->Press(InputID::Right))                             model_.pos.x += move;
    if (controller_->Press(InputID::Left))                              model_.pos.x -= move;
    if (controller_->Press(InputID::Up) && is2DOperation_)              model_.pos.y += move;
    if (controller_->Press(InputID::Down) && is2DOperation_)            model_.pos.y -= move;
    if (controller_->Press(InputID::Up) && !is2DOperation_)             model_.pos.z += move;
    if (controller_->Press(InputID::Down) && !is2DOperation_)           model_.pos.z -= move;
    MV1SetPosition(model_.handle, VGet(model_.pos.x, model_.pos.y, model_.pos.z));      // 座標
    MV1SetScale(model_.handle, VGet(model_.size.x, model_.size.y, model_.size.z));      // サイズ
    MV1SetRotationXYZ(model_.handle, VGet(0, model_.angle, 0));                         // 回転

    // カメラ移動(注視点)
    if (controller_->Press(InputID::CameraRight))                       camera_.target.x += move;
    if (controller_->Press(InputID::CameraLeft))                        camera_.target.x -= move;
    if (controller_->Press(InputID::CameraUp) && is2DOperation_)        camera_.target.y += move;
    if (controller_->Press(InputID::CameraDown) && is2DOperation_)      camera_.target.y -= move;
    if (controller_->Press(InputID::CameraUp) && !is2DOperation_)       camera_.target.z += move;
    if (controller_->Press(InputID::CameraDown) && !is2DOperation_)     camera_.target.z -= move;
    VECTOR offset = VTransform(VGet(0, 100, -500), MMult(MGetRotX(camera_.rot.x), MGetRotY(camera_.rot.y)));          // 行列を使った座標計算
    camera_.pos = (camera_.angle + camera_.target) + offset;

    if (CheckHitKey(KEY_INPUT_1))       post_.flag = true;                                 // ポストプロセス有効
    if (CheckHitKey(KEY_INPUT_2))       post_.flag = false;                                // ポストプロセス無効
    if (CheckHitKey(KEY_INPUT_3))       SetUsePixelShader(pixel_.tex);
    if (CheckHitKey(KEY_INPUT_4))       SetUsePixelShader(pixel_.lam);
    if (CheckHitKey(KEY_INPUT_5))       SetUsePixelShader(pixel_.toon);
    if (CheckHitKey(KEY_INPUT_6))       fieldFlag_ = true;
    if (CheckHitKey(KEY_INPUT_7))       fieldFlag_ = false;

    DrawOwnScreen(delta);
    return ownScene;
}

void ViewerScene::DrawOwnScreen(float delta)
{
    // 影用深度画像生成
    CreateShadowMap();
    // モデルの描画(シャドーマップ込み)
    DrawOffScreen();
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    // ポストプロセス
    DrawPostProcess(Vector2(0, 0), post_.tex, post_.ps[1], post_.flag);
    DrawFormatString(10, 10, 0xffffffff, "FPS=%f", GetFPS());
    // シャドウマップ(左上画面)
    //DrawRotaGraph(133, 83, 0.25, 0.0, ShadowMap_, true);
    ScreenFlip();
}

void ViewerScene::Relese(void)
{
}

void ViewerScene::CreateShadowMap(void)
{
    // 描画先を影用深度記録画像にする
    SetDrawScreen(shadowMap_.tex);
    // 影用深度記録画像を真っ白にする
    SetBackgroundColor(255, 255, 255);
    ClearDrawScreen();
    SetBackgroundColor(0, 0, 0);

    // カメラ
    SetupCamera_Ortho(camera_.ortPro);                      // カメラのタイプを正射影タイプセット、描画範囲も指定
    SetCameraNearFar(camera_.disNear, camera_.disFar);      // 描画する奥行範囲をセット
    auto lightPos = VAdd(VGet(camera_.target.x, camera_.target.y, camera_.target.z), VScale(GetLightDirection(), -2000));        // ライト座標
    SetCameraPositionAndTarget_UpVecY(lightPos, VGet(camera_.target.x, camera_.target.y, camera_.target.z));                     // カメラの視点、注視点の設定

    // シェーダー
    MV1SetUseOrigShader(true);                                          // モデルの描画に SetUseVertexShader, SetUsePixelShader を使用できるようにする
    // 深度値ピクセルシェーダー
    SetUsePixelShader(shadowMap_.ps);                                  // 深度値への描画用のピクセルシェーダーをセット
    SetUseVertexShader(shadowMap_.vs[0]);                              // 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
    // ステージ描画
    MV1DrawModel(stage_.handle);
    // 深度地記録画像へのスキニングメッシュ描画用の頂点シェーダーをセット
    SetUseVertexShader(shadowMap_.vs[1]);
    // モデル描画
    MV1DrawModel(model_.handle);
    MV1SetUseOrigShader(false);                                         // モデルの描画に SetUseVertexShader, SetUsePixelShader を使用できないように戻す
    // ライトの座標をもらっておく(カメラがライトの情報をもっている)
    lightM_.view = GetCameraViewMatrix();
    lightM_.projection = GetCameraProjectionMatrix();
}

void ViewerScene::DrawOffScreen(void)
{
    SetDrawScreen(post_.tex);
    ClsDrawScreen();

    // カメラ設定
    SetCameraPositionAndTarget_UpVecY(VGet(camera_.pos.x, camera_.pos.y + 100, camera_.pos.z - 300), VGet(camera_.target.x, camera_.target.y, camera_.target.z));

    // シェーダー
    MV1SetUseOrigShader(true);                  // モデルの描画に SetUseVertexShader, SetUsePixelShader を使用できるようにする
    // ライト
    lightMat_[0] = lightM_;
    // 定数バッファ
    UpdateShaderConstantBuffer(cBuf_.vs);                              // cbufferVSの変更を適用
    SetShaderConstantBuffer(cBuf_.vs, DX_SHADERTYPE_VERTEX, 4);        // cbufferVSを頂点シェーダーにセット
    SetUsePixelShader(shadowMap_.setPS);                               // 深度記録画面を使った影＋ディレクショナルライト１つ描画用のピクセルシェーダをセット
    // 影関係
    SetUseTextureToShader(1, shadowMap_.tex);                          // 影用深度記録画像をテクスチャ１にセット
    // ステージ描画
    SetUseVertexShader(stage_.mesh);
    MV1DrawModel(stage_.handle);
    // モデル描画
    SetUseVertexShader(model_.mesh);
    MV1DrawModel(model_.handle);
    MV1SetUseOrigShader(false);                  // モデルの描画に SetUseVertexShader, SetUsePixelShader を使用できないように戻す

    // テクスチャ解除
    SetUseTextureToShader(1, -1);
    SetUseTextureToShader(3, -1);
    // 設定した定数を解除
    
    // フィールドと軸描画
    if (fieldFlag_)
    {
        DrawFilde();
        DrawAxis();
    }
}

void ViewerScene::DrawPostProcess(Vector2 pos, int offScreen, int shader, bool flag)
{
    if (flag)
    {
        std::array<VERTEX2DSHADER, 4> verts{};                  // 2D描画に使用する頂点データ
        for (auto& v : verts)
        {
            // 初期化
            v.pos.z = 0.0f;                                     // Z座標
            v.rhw = 1.0;                                        // 同次 W の逆数
            v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);         // ディフューズ
            v.spc = GetColorU8(0xff, 0xff, 0xff, 0xff);		    // スペキュラ
            v.su = 0.0f;                                        // 補助テクスチャ横方向座標
            v.sv = 0.0f;                                        // 補助テクスチャ縦方向座標
        }

        // 画面サイズ取得
        int width, height;
        GetGraphSize(offScreen, &width, &height);
        // 頂点シェーダー情報格納
        verts[0] = StorageVertex(verts[0], pos, Vector2(0.0f, 0.0f));                                       // 左上
        verts[1] = StorageVertex(verts[1], Vector2(pos.x + width, pos.y), Vector2(1.0f, 0.0f));             // 右上
        verts[2] = StorageVertex(verts[2], Vector2(pos.x, pos.y + height), Vector2(0.0f, 1.0f));            // 左下
        verts[3] = StorageVertex(verts[3], Vector2(pos.x + width, pos.y + height), Vector2(1.0f, 1.0f));    // 右下

        SetUsePixelShader(shader);                                                                          // ピクセルシェーダー設定
        SetUseTextureToShader(0, offScreen);                                                                // シェーダーハンドル設定
        DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);                     // シェーダーを使って2Dプリミティブを描画
    }
    else
    {
        // ポストプロセスなしのオフスクリーン
        DrawGraph(pos.x, pos.y, offScreen, true);
    }
    // テクスチャ解除
    SetUseTextureToShader(0, -1);
}

VERTEX2DSHADER ViewerScene::StorageVertex(VERTEX2DSHADER vert, Vector2 pos, Vector2 texPos)
{
    vert.pos.x = pos.x;
    vert.pos.y = pos.y;
    vert.u = texPos.x;
    vert.v = texPos.y;
    return vert;
}

void ViewerScene::DrawAxis(void)
{
    // ワールド座標表示
    DrawLine3D(VGet(0, 0, 0), VGet(100, 0, 0), 0xff0000);           // X座標
    DrawLine3D(VGet(0, 0, 0), VGet(0, 100, 0), 0x00ff00);           // Y座標
    DrawLine3D(VGet(0, 0, 0), VGet(0, 0, -100), 0x0000ff);          // Z座標
}

void ViewerScene::DrawFilde(void)
{
    // フィールド線表示
    for (int num = -rayNum; num <= rayNum; num++)
    {
        auto ray = rayNum * num;                               // 線の間隔
        auto length = Square(rayNum);                          // 線の長さ
        DrawLine3D(VGet(ray, 0, -length), VGet(ray, 0, length), 0x00ff00);              // 横線
        DrawLine3D(VGet(-length, 0, ray), VGet(length, 0, ray), 0x00ff00);              // 縦線
    }
}
