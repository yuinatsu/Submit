#include "SceneMng.h"
#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"
#include "EditScene.h"
#include "GameScene.h"


void SceneMng::Init(void)
{
	SysInit();										// システム系		// 重要な内容が書き込まれているのでコンストラクタ内に書いてもよい
	scene_ = std::make_unique<EditScene>();			// 初期シーン設定
}


void SceneMng::Run(void)
{
	Init();											// 初期化

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		scene_ = scene_->Update(std::move(scene_));					// シーン更新		// std::move ... （）内のものを移動
		Draw();														// シーン描画
	}
}


void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);			// 描画する画面を裏の画面に設定
	ClearDrawScreen();						// 描画した画面をクリア

	scene_->Draw();							// 描画

	_dbgAddDraw();							// デバッグ描画
	ScreenFlip();
}


// システム系初期化
//-------------------
bool SceneMng::SysInit(void)
{
	// システム処理
	SetWindowText("ロードランナー");						// ゲームウィンドウのタイトル
	SetGraphMode(scnArea_.x, scnArea_.y, 16);				// ゲームウィンドウのサイズと色モードを設定
	ChangeWindowMode(true);									// ゲームウィンドウの表示方法 (true : window   false : フルスクリーン)

	if (DxLib_Init() == -1)									// DXライブラリの初期化処理
	{
		TRACE("DxLib の初期化失敗");
		return false;										// DXライブラリの初期化失敗の為システム終了
	}

	SetDrawScreen(DX_SCREEN_BACK);							// 描画先画面を設定
	_dbgSetup(scnArea_.x, scnArea_.y, 255);					// デバッグを設定

	return true;
}


#define VIEW_AREA_X 21										// ステージ中の描画エリアX座標
#define VIEW_AREA_Y 17										// ステージ中の描画エリアY座標


SceneMng::SceneMng() : scnArea_{ 800,600 }, worldArea_{ VIEW_AREA_X * 2,VIEW_AREA_Y }, viewArea_{ VIEW_AREA_X,VIEW_AREA_Y }, chipSize_{ 32,32 }, drawOffset_{ (scnArea_ - (viewArea_ * chipSize_)) / 2 }					// スクリーンエリアサイズ定義
{

}


SceneMng::~SceneMng()
{

}
