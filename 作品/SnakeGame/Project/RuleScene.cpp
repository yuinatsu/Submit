#include <DxLib.h>
#include "RuleScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// RuleSceneの生成
//------------------
RuleScene::RuleScene()
{
	TRACE("RuleSceneの生成\n");
}

// RuleSceneの破棄
//------------------
RuleScene::~RuleScene()
{
	TRACE("RuleSceneの破棄\n");
}

// RuleSceneの更新
//------------------
SCN_ID RuleScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("RuleScene_Update\n");

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])					// SPACEを押すとセレクト画面に移動
	{
		return SCN_ID::SELECT;
	}
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])						// 強制タイトル移動
	{
		return SCN_ID::TITLE;
	}
	return SCN_ID::RULE;
}

// RuleSceneの描画
//------------------
void RuleScene::Draw(void)
{
	TRACE("RuleScene_Draw\n");

	if ((blinkCnt_ / 50) % 2)													// 点滅
	{
		DrawExtendGraph(spacePos_.x, spacePos_.y, spacePos_.x + SPACE_IMAGE_SIZE_X / 2, spacePos_.y + SPACE_IMAGE_SIZE_Y / 2, SpaceImage_, true);				// "PUSH SPACE KEY"(縮小)
	}
	blinkCnt_++;
}

// RuleSceneの初期化
//--------------------
bool RuleScene::Init(void)
{
	TRACE("RuleScene_Init\n");

	bool rtnFlag = true;
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"の読み込み
	{
		TRACE("読み込みエラー\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// スクリーンサイズ取得

	spacePos_.x = screenSize_.x - SPACE_IMAGE_SIZE_X  * 2 / 3;					// "PUSH SPACE KEY"のX座標
	spacePos_.y = screenSize_.y - SPACE_IMAGE_SIZE_Y;							// "PUSH SPACE KEY"のY座標

	return rtnFlag;
}

// RuleSceneの解放
//------------------
bool RuleScene::Relese(void)
{
	TRACE("RuleScene_Relese\n");

	DeleteGraph(SpaceImage_);													// "PUSH SPACE KEY"の解放
	return true;
}
