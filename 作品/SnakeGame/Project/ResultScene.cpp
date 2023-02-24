#include <DxLib.h>
#include "ResultScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

#define WIN_MES_IMAGE_SIZE_X 400													// 勝利メッセージ画像の横サイズ
#define WIN_MES_IMAGE_SIZE_Y (300 / static_cast<int>(CHAR_ID::MAX))					// 勝利メッセージ画像の縦サイズ

// ResultSceneの生成
//--------------------
ResultScene::ResultScene()
{
	TRACE("ResultSceneの生成\n");
}

// ResultSceneの破棄
//--------------------
ResultScene::~ResultScene()
{
	TRACE("ResultSceneの破棄\n");
}

// ResultSceneの更新
//--------------------
SCN_ID ResultScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("ResultScene_Update\n");

	if (finishCnt_ < FINISH_COUNT_MAX)					// 一定数カウントを行っている間RESULTに返り続ける
	{
		finishCnt_++;
		return SCN_ID::RESULT;
	}

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])				// SPACEを押してタイトルに戻る
	{
		return SCN_ID::TITLE;
	}
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])					// 強制タイトル移動
	{
		return SCN_ID::TITLE;
	}
	return SCN_ID::RESULT;
}

// ResultSceneの描画
//--------------------
void ResultScene::Draw(void)
{
	TRACE("ResultScene_Draw\n");

	DrawGraph(0, picPos_.y + CHAR_PIC_SIZE_Y / 3, winMesImage[static_cast<int>(winnerID_)], true);				// 勝利者メッセージ			// bgImageより上に記述することによってカウントが終わるまでは表示されないように見える

	switch (winnerID_)																							// キャラ立ち絵
	{
	case CHAR_ID::REIMU:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][0], true);
		break;
	case CHAR_ID::MARISA:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][1], true);
		break;
	case CHAR_ID::CIRNO:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][2], true);
		break;
	case CHAR_ID::SAKUYA:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][3], true);
		break;
	case CHAR_ID::YOUMU:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][4], true);
		break;
	case CHAR_ID::DRAW:
		break;
	default:
		break;
	}

	if ((blinkCnt_ / 50) % 2)													// 点滅
	{
		DrawExtendGraph(spacePos_.x, spacePos_.y, spacePos_.x + SPACE_IMAGE_SIZE_X / 2, spacePos_.y + SPACE_IMAGE_SIZE_Y / 2, SpaceImage_, true);				// "PUSH SPACE KEY"(縮小)
	}
	blinkCnt_++;

	if (!(finishCnt_ >= FINISH_COUNT_MAX))																		// 一定数ゲーム背景を表示
	{
		if (finishCnt_ > 0)
		{
			DrawGraph(0, 0, bgImage_, true);		// 背景描画
		}
	}
}

// ResultSceneの初期化
//----------------------
bool ResultScene::Init(CHAR_ID winnerID)
{
	TRACE("ResultScene_Init\n");

	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);						// スクリーン描画を取得
	bgImage_ = MakeScreen(screenSize_.x, screenSize_.y, false);				// bgImageに格納
	GetDrawScreenGraph(0, 0, screenSize_.x, screenSize_.y, bgImage_);		// ゲーム背景の描画

	charImage_.Init(winnerID);													// charImageのInitを呼び出し

	bool rtnFlag = true;
	if ((LoadDivGraph("Resource/image/win.png", static_cast<int>(CHAR_ID::MAX), 1, static_cast<int>(CHAR_ID::MAX), WIN_MES_IMAGE_SIZE_X, WIN_MES_IMAGE_SIZE_Y, winMesImage)) == -1)				// 勝利者メッセージ
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"の読み込み
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// スクリーンサイズの取得

	picPos_.x = (screenSize_.x - CHAR_PIC_SIZE_X) * 5 / 6;						// キャラ立ち絵X座標
	picPos_.y = (screenSize_.y - CHAR_PIC_SIZE_Y) / 2;							// キャラ立ち絵Y座標

	spacePos_.x = (screenSize_.x - SPACE_IMAGE_SIZE_X) * 2 / 5;					// "PUSH SPACE KEY"のX座標
	spacePos_.y = (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 3 / 5;					// "PUSH SPACE KEY"のY座標

	finishCnt_ = 0;																// 合図買う運他の初期化

	winnerID_ = winnerID;														// gameSceneから送られてきたwinnerIDをwinnerID_に格納

	return rtnFlag;
}

// ResultSceneの解放
//--------------------
bool ResultScene::Relese(void)
{
	TRACE("ResultScene_Relese\n");

	DeleteGraph(SpaceImage_);													// "PUSH SPACE KEY"の解放

	for (int no = 0; no < static_cast<int>(CHAR_ID::MAX); no++)					// 勝利者メッセージの解放
	{
		DeleteGraph(winMesImage[no]);
	}

	for (int no = 0; no < PLAYER_MAX; no++)										// playerの解放
	{
		player[no].Relese();
	}

	DeleteGraph(bgImage_);														// ゲーム背景

	return true;
}
