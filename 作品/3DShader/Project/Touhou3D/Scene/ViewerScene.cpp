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
    // モデル関係初期化
    InitModel();
    // カメラ初期化
    InitCamera();
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

    attachIndex_ = MV1AttachAnim(model_.handle, 0, -1, false);
    totalTime_ = MV1GetAttachAnimTotalTime(model_.handle, attachIndex_);
    playTime_ = 0.0f;

    return true;
}

void ViewerScene::InitCamera(void)
{
    // カメラ
    camera_.target = model_.pos + Vector3(0.0f, 150.0f, -100.0f);
    camera_.pos = zeroVector3<float>;
    camera_.rot = zeroVector3<float>;
    camera_.angle = zeroVector3<float>;
}

void ViewerScene::InitModel(void)
{
    // モデル
    model_.handle = MV1LoadModel("./Resource/Model/十六夜咲夜/sakuya.mv1");
    model_.pos = Vector3(0.0f, 100.0f, -100.0f);
    model_.angle = Deg2Rad(180.0f);
    model_.size = Vector3(10.0f, 10.0f, 10.0f);
    // ステージ
    stage_.handle = MV1LoadModel("./Resource/Stage/人里/人里.mv1");
    stage_.size = Vector3(100.0f, 100.0f, 100.0f);
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
    shadowMap_.vs[0] = LoadVertexShader("Resource/Shader/ShadowMap/Mesh1Shadow.vso");
    shadowMap_.vs[1] = LoadVertexShader("Resource/Shader/ShadowMap/Mesh4Shadow.vso");
    shadowMap_.ps = LoadPixelShader("Resource/Shader/ShadowMap/ShadowMapPS.pso");
    shadowMap_.setPS = LoadPixelShader("Resource/Shader/Shadow/SetShadowMap.pso");

    // ステージ
    stage_.mesh = LoadVertexShader("Resource/Shader/Shadow/StageVS.vso");
    // モデル
    model_.mesh = LoadVertexShader("Resource/Shader/Shadow/ModelVS.vso");

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
    post_.ps[0] = LoadPixelShader("Resource/Shader/PostEffect/Aveblur.pso");            // ブラー
    post_.ps[1] = LoadPixelShader("Resource/Shader/PostEffect/Mono.pso");               // モノトーンカラー
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
    MV1SetScale(stage_.handle, VGet(stage_.size.x, stage_.size.y, stage_.size.z));
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
        vs_ = LoadVertexShader("Resource/Shader/Vertex/Mesh1.vso");
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/Mesh4.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/NormMesh1.vso");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        vs_ = LoadVertexShader("Resource/Shader/Vertex/NormMesh4.vso");
        break;
    default:
        break;
    }
    // 法線マップ用の情報が含まれているか
    if (vertType <= DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
    {
        pixel_.tex = LoadPixelShader("Resource/Shader/Pixel/Tex.pso");
        pixel_.lam = LoadPixelShader("Resource/Shader/Pixel/Lambert.pso");
        pixel_.toon = LoadPixelShader("Resource/Shader/Pixel/Toon.pso");
    }
    else
    {
        pixel_.tex = LoadPixelShader("Resource/Shader/Pixel/NormTex.pso");
        pixel_.lam = LoadPixelShader("Resource/Shader/Pixel/NormLambert.pso");
        pixel_.toon = LoadPixelShader("Resource/Shader/Pixel/NormToon.pso");
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
    rot_ = controller_->GetLeftInput().GetAngle();
    // モデル移動
    ProcessMove();
    // モデル回転
    ProcessRotate();

    // カメラ回転
    if (controller_->Press(InputID::CameraRight))
    {
        camera_.rot.z += 100.0f;
    }
    if (controller_->Press(InputID::CameraLeft))
    {
        camera_.rot.z -= 100.0f;
    }

    // モデル
    MV1SetPosition(model_.handle, VGet(model_.pos.x, model_.pos.y, model_.pos.z));      // 座標
    MV1SetScale(model_.handle, VGet(model_.size.x, model_.size.y, model_.size.z));      // サイズ
    MV1SetRotationXYZ(model_.handle, VGet(0, model_.angle, 0));                         // 回転
    // カメラ
    VECTOR offset = VTransform(VGet(0, 100, -500), MMult(MGetRotX(camera_.rot.x), MGetRotY(camera_.rot.y)));          // 行列を使った座標計算
    camera_.pos = camera_.angle + camera_.target;                                          // カメラ座標(プレイヤーに同期)


    if (CheckHitKey(KEY_INPUT_1))       post_.flag = true;                                 // ポストプロセス有効
    if (CheckHitKey(KEY_INPUT_2))       post_.flag = false;                                // ポストプロセス無効
    if (CheckHitKey(KEY_INPUT_3))       SetUsePixelShader(pixel_.tex);
    if (CheckHitKey(KEY_INPUT_4))       SetUsePixelShader(pixel_.lam);
    if (CheckHitKey(KEY_INPUT_5))       SetUsePixelShader(pixel_.toon);

    // アニメーション
    playTime_ += 0.8f;
    if (playTime_ >= totalTime_)
    {
        playTime_ = 0.0f;
    }
    MV1SetAttachAnimTime(model_.handle, attachIndex_, playTime_);

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
    DrawPostProcess(Vector2(0, 0), post_.tex, post_.ps[0], post_.flag);
    DrawFormatString(10, 10, 0xffffffff, "FPS=%f", GetFPS());
    // シャドウマップ(左上画面)
    //DrawRotaGraph(133, 83, 0.25, 0.0, shadowMap_.tex, true);
    ScreenFlip();
}

void ViewerScene::Relese(void)
{
}

void ViewerScene::ProcessMove(void)
{
    // 移動量
    auto move = moveValue;
    if (controller_->Press(InputID::Dash))          move *= 3;

    // 通常移動
    if (controller_->GetLeftInput().SqMagnitude() > 0.0f)
    {
        auto camAngle = ToEuler(camera_.rot);
        model_.pos.x -= sinf(camAngle.y + rot_) * move;
        model_.pos.z -= cosf(camAngle.y + rot_) * move;
        camera_.target.x -= sinf(camAngle.y + rot_) * move;
        camera_.target.z -= cosf(camAngle.y + rot_) * move;
    }
}

void ViewerScene::ProcessRotate(void)
{
    // モデルの角度を設定
    if (!(controller_->GetLeftInput().SqMagnitude() > 0.0f))
    {
        return;
    }

    // 移動方向にキャラクターの角度を徐々に変える
    float radUnitAnglesY = model_.angle;
    auto camAngle = ToEuler(camera_.rot);

    float radMoveAnglesY = RadIn2PI(camAngle.y + rot_);

    // 回転量が少ない方の回転方向を取得する（時計回り：１、反時計回り：ー１）
    float aroundDir = DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

    // true:しきい値以内に合わせる false:回転量を加える
    float diff = abs(radMoveAnglesY - radUnitAnglesY);
    auto rotRad = 5.0f * (DX_PI_F / 180.0f);
    model_.angle = (diff < 0.1f) ? radMoveAnglesY : model_.angle + (rotRad * aroundDir);

    // angles_.yも0~360度以内に収める
    model_.angle = RadIn2PI(model_.angle);
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
