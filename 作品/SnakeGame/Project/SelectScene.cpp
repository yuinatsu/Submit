#include <DxLib.h>
#include "SelectScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// SelestSceneの生成
//--------------------
SelectScene::SelectScene()
{
	TRACE("SelectSceneの生成\n");
}

// SelestSceneの破棄
//--------------------
SelectScene::~SelectScene()
{
	TRACE("SelectSceneの破棄\n");
}

// SelestSceneの更新
//--------------------
SCN_ID SelectScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("SelectScene_Update\n");

	for (int no = 0; no < PLAYER_MAX; no++)												// プレイヤー数分
	{
		player[no].GetKeyState(keyData);												// プレイヤーのキーデータ取得

		SelectChar(&player[no], DIR::LEFT, -1, CHAR_ID::REIMU, CHAR_ID::YOUMU);			// 左キーを押してREIMU,MARISA,CIRNO,SAKUYA,YOUMUの順でキャラセレクト		// ポインタ渡しをする際は & を頭につける
		SelectChar(&player[no], DIR::RIGHT, 1, CHAR_ID::YOUMU, CHAR_ID::REIMU);			// 右キーを押してYOUMU,SAKUYA,CIRNO,MARISA,REIMUの順でキャラセレクト

		DesideChar(&player[no], DIR::UP, PL_ST::ALIVE);									// 上キーを押してキャラセレクト決定
		DesideChar(&player[no], DIR::DOWN, PL_ST::NON);									// 下キーを押してキャラセレクトキャンセル
	}

	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])							// 強制タイトル移動
	{
		return SCN_ID::TITLE;
	}
	if ((player[0].state_ == PL_ST::ALIVE) && (player[1].state_ == PL_ST::ALIVE))		// 全員キャラが決定したらゲームシーンに移動
	{
		return SCN_ID::GAME;
	}
	return SCN_ID::SELECT;
}

// キャラセレクトキー入力関数 
//-----------------------------
bool SelectScene::SelectChar(Player* player, DIR dir, int count, CHAR_ID limID, CHAR_ID setID)		// Playerの後ろに * を付けることでポインタ渡しを行う
{
	if (player->state_ != PL_ST::NON)																// PL_STがNON以外だったらリターンする (NON以外でキー入力処理をしたくないから)
	{
		return false;
	}

	if (player->keyNow_[static_cast<int>(dir)] && !(player->keyOld_[static_cast<int>(dir)]))		// 左右キーを押してキャラセレクト			-> ....アロー文字    ポインタを使うときに使う
	{
		if (player->charID_ != limID)
		{
			player->charID_ = static_cast<CHAR_ID>(static_cast<int>(player->charID_) + count);		// 最大値に達していない場合キャラ描画スクロール
		}
		else
		{
			player->charID_ = setID;																// 最大値に達していた場合最小値に移動
		}
		return true;
	}
	return false;
}

// キャラ決定キー入力関数
//-------------------------
bool SelectScene::DesideChar(Player* player, DIR dir, PL_ST state)
{
	if (player->keyNow_[static_cast<int>(dir)] && !(player->keyOld_[static_cast<int>(dir)]))		// 上下キーを押してキャラ状態変化			// 上：決定、下：キャンセル
	{
		player->state_ = state;																		// 状態変化
		return true;
	}
	return false;																					// キャラが上下キーが押されるまでDesideCharは処理する必要がないのでfalseを返す
}

// SelestSceneの描画
//--------------------
void SelectScene::Draw(void)
{
	TRACE("SelectScene_Draw\n");

	DrawGraph(0, 0, bgImage_, true);																		// ゲーム背景の描画
	DrawGraph((screenSize_.x - CHAR_SEL_IMAGE_SIZE_X) / 2, 0, logoImage_, true);							// キャラ選択中のロゴの描画
	for (int no = 0; no < PLAYER_MAX; no++)																	// プレイヤーごとの選択中キャラ
	{
		DrawGraph(DRAW_OFFSET_X + player[no].pos_.x, DRAW_OFFSET_Y + player[no].pos_.y - CHIP_SIZE / 3, charImage[static_cast<int>(player[no].charID_)].chipImage[static_cast<int>(DIR::DOWN)][0], true);
	}
}

// SelestSceneの初期化
//----------------------
bool SelectScene::Init(void)
{
	TRACE("SelectScene_Init\n");

	bool rtnFlag = true;
	for (int j = 0; j < static_cast<int>(CHAR_ID::MAX); j++)				// 選択用のキャラ
	{
		charImage[j].Init(static_cast<CHAR_ID>(j));
	}
	if ((bgImage_ = LoadGraph("Resource/image/bg.png")) == -1)				// ゲーム背景
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}
	if ((logoImage_ = LoadGraph("Resource/image/char_sel.png")) == -1)		// キャラセレクトロゴ
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}
	for (int no = 0; no < PLAYER_MAX; no++)									// キャラごとの状態
	{
		player[no].state_ = PL_ST::NON;
	}
	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);				// スクリーンサイズの取得
	return rtnFlag;
}

// SelestSceneの解放
//--------------------
bool SelectScene::Relese(void)
{
	TRACE("SelectScene_Relese\n");

	for (int j = 0; j < static_cast<int>(CHAR_ID::MAX); j++)				// charImageの解放
	{
		charImage[j].Release();
	}

	player[0].Relese();					// playerの解放// 将来的にキャラ数を増やすならここはfor文でまわす
	player[1].Relese();					// playerの解放

	DeleteGraph(logoImage_);			// キャラセレクトロゴの解放
	DeleteGraph(bgImage_);				// ゲーム背景の解放

	return true;
}
