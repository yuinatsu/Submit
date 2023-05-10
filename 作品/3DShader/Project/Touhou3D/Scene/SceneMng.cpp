#include <DxLib.h>
#include <memory>
#include <chrono>
#include "SceneMng.h"
#include "ViewerScene.h"
#include "../Common/Debug.h"
#include "../Input/InputConfig.h"

// スクリーンサイズ
static constexpr Vector2I screenSize{ 1280, 720 };

void SceneMng::Run(void)
{
	// システム系初期化
	if (!SysInit())
	{
		return;
	}
	// 初期化処理
	Init();
	// メインループ
	while (ProcessMessage() == 0)
	{
		// 更新処理
		Update();
		// 描画処理
		Draw();
	}
	// 終了処理
	End();
}

bool SceneMng::SysInit(void)
{
	// デバッグ開始	Debug.h
	DebugStart(time_);
	// ウィンドウテキスト
	SetWindowText("3DViewer");
	// 画面モード設定
	SetGraphMode(screenSize.x, screenSize.y, 32);
	// ウィンドウモード
	ChangeWindowMode(true);
	// Direct3Dのバージョン設定
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	// Zバッファを使用するか
	SetUseZBuffer3D(true);
	// Zバッファに書き込みを行うか
	SetWriteZBuffer3D(true);

	// DXLIBの初期化
	if (DxLib_Init() == -1)
	{
		return false;
	}
	// キーコンフィグ生成
	InputConfig::Create();
	// デバッグセットアップ	Debug.h
	DebugSetUp();

	return true;
}

void SceneMng::Init(void)
{
	// シーン初期設定
	scene_ = std::make_unique<ViewerScene>();

	// デルタタイム初期化
	time_.DeltaTimeStart();
	time_.DeltaTimeEnd();
	time_.GameTimeEnd();
}

void SceneMng::Update(void)
{
	time_.DeltaTimeStart();

	// シーン更新
	scene_ = scene_->Update(std::move(scene_), time_.GetDeltaTime<float>());

	time_.GameTimeEnd();
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	// シーン描画
	scene_->Draw();
	// デバッグ情報描画
	DebugInfoDraw();

	ScreenFlip();
	time_.DeltaTimeEnd();
}

void SceneMng::End(void)
{
	// 全てのグラフィックハンドルを削除
	InitGraph();
	// キーコンフィグの削除
	InputConfig::Destroy();
	// DXLIB使用終了
	DxLib_End();
}
