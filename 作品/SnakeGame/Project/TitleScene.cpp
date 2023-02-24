#include <DxLib.h>
#include "TitleScene.h"
#include "_debug/_DebugConOut.h"

// TitleSceneの生成
//-------------------
TitleScene::TitleScene()
{
	TRACE("TitleSceneの生成\n");
}

// TitleSceneの破棄
//-------------------
TitleScene::~TitleScene()
{
	TRACE("TitleSceneの破棄\n");
}

// TitleSceneの更新
//-------------------
SCN_ID TitleScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("TitleScene_Update\n");
	
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])			// SPACEキーを押すとstartFlagをtrueにする				// if(startFlag_ == true)の中身をこの中で書くとSPACEキーを何回も押す必要が出てくる
	{
		startFlag_ = true;
	}
	if (startFlag_ == true)													// SPACEキーを押してstartFlagがtrueになったら"PUSH SPACE KEY"の高速点滅を開始
	{
		if (startCnt_ < START_COUNT_MAX)									// カウントを一定数加算
		{
			startCnt_++;
			return SCN_ID::TITLE;											// 一定数のカウントを超えるまではIDをTITLEで回す
		}
		startFlag_ = false;
		return SCN_ID::RULE;												// 高速点滅を終えたらルール画面に移動
	}
	return SCN_ID::TITLE;
}

// TitleSceneの描画
//-------------------
void TitleScene::Draw(void)
{
	TRACE("TitleScene_Draw\n");

	DrawGraph(0, 0, TitleBgImage_, true);											// タイトル背景の描画
	DrawGraph((screenSize_.x - TITLE_IMAGE_SIZE_X) / 2, 0, TitleImage_, true);		// タイトルロゴの描画
	
	if (startFlag_ == false)														// SPACEキーを押されるまでは通常の速さで点滅
	{
		if ((blinkCnt_ / 50) % 2)													// 通常点滅
		{
			DrawGraph((screenSize_.x - SPACE_IMAGE_SIZE_X) / 2, (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 2 / 3, SpaceImage_, true);			// "PUSH SPACE KEY"の描画
		}
	}
	else
	{
		if ((blinkCnt_ / 10) % 2)													// 高速点滅
		{
			DrawGraph((screenSize_.x - SPACE_IMAGE_SIZE_X) / 2, (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 2 / 3, SpaceImage_, true);			// "PUSH SPACE KEY"の描画
		}
	}
	blinkCnt_++;																// カウントアップ
	
	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// スクリーンサイズの取得
}

// TitleSceneの初期化
//---------------------
bool TitleScene::Init(void)
{
	TRACE("TitleScene_Init\n");

	bool rtnFlag = true;
	if ((TitleBgImage_ = LoadGraph("Resource/image/titlebg.jpg")) == -1)			// タイトルロゴの読み込み
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}
	if ((TitleImage_ = LoadGraph("Resource/image/title.png")) == -1)			// タイトルロゴの読み込み
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"の読み込み
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}

	// カウンタの初期化
	startCnt_ = 0;
	blinkCnt_ = 0;

	return rtnFlag;
}

// TitleSceneの解放
//-------------------
bool TitleScene::Relese(void)
{
	TRACE("TitleScene_Relese\n");

	DeleteGraph(SpaceImage_);						// "PUSH SPACE KEY"の解放
	DeleteGraph(TitleImage_);						// タイトルロゴの解放
	DeleteGraph(TitleBgImage_);						// タイトル背景の解放

	return true;
}
