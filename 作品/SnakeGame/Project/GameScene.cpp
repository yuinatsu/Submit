#include <DxLib.h>
#include "GameScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// GameSceneの生成
//------------------
GameScene::GameScene()
{
	TRACE("GameSceneの生成\n");
}

// GameSceneの破棄
//------------------
GameScene::~GameScene()
{
	TRACE("GameSceneの破棄\n");
}

// GameSceneの更新
//------------------
SCN_ID GameScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("GameScene_Update\n");

	for (int cnt = 0; cnt < START_CALL_MAX; cnt++)						// call.pngの上２枚分
	{
		if (callCnt_ < CALL_COUNT_MAX + (CALL_COUNT_MAX * cnt))			// カウントを一定数加算
		{
			callCnt_++;
			return SCN_ID::GAME;										// 一定数のカウントを超えるまではIDをGAMEで回す
		}
	}
	if (callCnt_ >= CALL_COUNT_MAX * 2)									// 一定時間経過でカウントダウン終了
	{
		callFlag_ = false;
	}

	mapCtl.Update();													// mapCtl.Update()の呼び出し

	int deadCnt = 0;																// state_がDEADのキャラのカウンタ
	for (int no = 0; no < PLAYER_MAX; no++)											// プレイヤーごと(今回は２人)のUpdateを呼び出し
	{
		if (player[no].Update(mapCtl, keyData, keyDataOld) == PL_ST::DEAD)			// プレイヤーのstate_がDEADだったらdeadCntをカウントアップ
		{
			deadCnt++;
		}
	}
	
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])				// 強制タイトル移動
	{
		return SCN_ID::TITLE;
	}
	if (deadCnt > 0)								// プレイヤーが１人でも死んでいたらリザルトシーンに移動
	{
		deadFlag_ = true;							// リセット
		callFlag_ = true;							// リセット
		return SCN_ID::RESULT;
	}
	return SCN_ID::GAME;							// プレイヤーがどちらも生きていたらゲームシーンを続行


}

// GameSceneの描画
//------------------
void GameScene::Draw(void)
{
	TRACE("GameScene_Draw\n");

	DrawGraph(0, 0, bgImage_, true);			// ゲーム背景

	mapCtl.Draw();								// mapCtl.Draw()の呼び出し

	for (int no = 0; no < PLAYER_MAX; no++)		// プレイヤーごとのキャラチップとキャラアイコン
	{
		player[no].Draw();
	}

	if (callFlag_ == true)
	{
		StartCallDraw();							// ゲーム開始前のカウントダウン表示
	}

	if (deadFlag_ == true)						// ゲーム終了表示
	{
		DrawGraph((screenSize_.x - CALL_IMAGE_SIZE_X) / 2, (screenSize_.y - CALL_IMAGE_SIZE_Y) / 2, callImage[2], true);
	}
}

// ゲーム開始前のカウントダウン表示
//-----------------------------------
void GameScene::StartCallDraw(void)
{
	for (int cnt = 0; cnt < START_CALL_MAX; cnt++)								// call.pngの上２枚分の表示
	{
		if (!(callCnt_ >= CALL_COUNT_MAX + (cnt * CALL_COUNT_MAX)))				// カウントが一定数行ったら表示しないようにする
		{
			if (callCnt_ >= 50 * (cnt + (cnt * 2)))								// callImageを一定数表示
			{
				DrawGraph((screenSize_.x - CALL_IMAGE_SIZE_X) / 2, (screenSize_.y - CALL_IMAGE_SIZE_Y) / 2, callImage[cnt], true);
			}
		}
	}
}

// GaneSceneの初期化
//--------------------
bool GameScene::Init(CHAR_ID char1, CHAR_ID char2)
{
	TRACE("GameScene_Init\n");

	bool rtnFlag = true;
	if ((bgImage_ = LoadGraph("Resource/image/bg.png")) == -1)				// ゲーム背景
	{
		TRACE("読み込みエラー1\n");
		rtnFlag = false;
	}
	if ((LoadDivGraph("Resource/image/call.png", CALL_MAX, 1, CALL_MAX, CALL_IMAGE_SIZE_X, CALL_IMAGE_SIZE_Y, callImage)) == -1)				// 合図ロゴ
	{
		TRACE("読み込みエラー2\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);				// スクリーンサイズ取得

	mapCtl.Init();															// mapCtlのInitを呼び出し		// 他のものから参照されやすいものからInitしていく

	player[0].Init(char1);													// player[0]にchar1の情報を入れる
	player[1].Init(char2);													// player[1]にchar2の情報を入れる

	callCnt_ = 0;															// カウントダウン用カウンタ
	deadFlag_ = false;														// 死亡フラグ

	return rtnFlag;
}

// GameSceneの解放
//------------------
bool GameScene::Relese(void)
{
	TRACE("GameScene_Relese\n");

	for (int no = 0; no < PLAYER_MAX; no++)						// プレイヤーごとのキャラ解放
	{
		player[no].Relese();
	}

	mapCtl.Relese();											// mapCtlの解放				// Releseする順番はInitした順番と逆

	for (int no = 0; no < CALL_MAX; no++)						// 合図ロゴの解放
	{
		DeleteGraph(callImage[no]);
	}

	DeleteGraph(bgImage_);										// ゲーム背景の解放

	return true;
}
