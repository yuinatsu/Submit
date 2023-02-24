#pragma warning(disable : 4996)	// fopenを簡易的に使用してます
#include <DxLib.h>
#include "SceneCtl.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

SceneCtl::SceneCtl() :scnArea_{ 720,480 }, TotalScore_{1000}, CVS{ 50 }//,totalscore(lpGameScene.TotalScore_)
{
	SysInit();
}

SceneCtl::~SceneCtl()
{
	Release();
}

//--------------------------------------------------------------------------
//●ゲームループ
//--------------------------------------------------------------------------
void SceneCtl::Run(void)
{
	Init();
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (ProcessMessage() != 0)
		{
			lpSaveLoad.DataSave();			// データセーブ
		}
		oldScnID_ = scnID_;
		if (scnID_ != nextScnID_)
		{
			SetupScene();	// 各シーンのInit
		}
		nextScnID_ = scnID_;

		GetKeyData();
		Update();
		if (nextScnID_ == scnID_)
		{
			Draw();
		}
	}
	Release();
}

//--------------------------------------------------------------------------
//●シーンの更新
//--------------------------------------------------------------------------
void SceneCtl::Update(void)
{
	// 新しいシーンの更新
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		nextScnID_ = titleScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::SELECT:
		nextScnID_ = selectScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GAME:
		nextScnID_ = gameScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::RESULT:
		nextScnID_ = resultScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GUIDE:
		nextScnID_ = guideScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::DATABASE:
		nextScnID_ = dataBaseScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::PERFORM:
		nextScnID_ = performScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::WEAPON:
		nextScnID_ = weaponScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::MAX:
		break;
	default:
		TRACE("未知のシーンID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
}

//--------------------------------------------------------------------------
//●グラフィックの描画
//--------------------------------------------------------------------------
void SceneCtl::Draw(void)
{
	_dbgStartDraw();

	SetDrawScreen(DX_SCREEN_BACK);	// バックバッファに設定
	ClsDrawScreen();	// 画面の初期化

	// 新しいシーンの描画
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Draw();
		break;
	case SCN_ID::SELECT:
		selectScene_.Draw();
		break;
	case SCN_ID::GAME:
		gameScene_.Draw();
		break;
	case SCN_ID::RESULT:
		resultScene_.Draw();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Draw();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Draw();
		break;
	case SCN_ID::PERFORM:
		performScene_.Draw();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Draw();
		break;
	case SCN_ID::MAX:
		break;
	default:
		TRACE("未知のシーンID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
	_dbgAddDraw();
	ScreenFlip();	// 裏画面の内容を表画面に反映させる
}

//--------------------------------------------------------------------------
//●初期化
//--------------------------------------------------------------------------
bool SceneCtl::Init(void)
{
	SysInit();

	lpSaveLoad.DataSave();		// ゲームを始める前に初期データをセットしておく
	lpSaveLoad.DataLoad();

	oldScnID_ = SCN_ID::MAX;		// oldの初期値は何でもいいけど取り敢えずMAXにしている
	scnID_ = SCN_ID::MAX;			// 現在（最初はまだ始まってないのでMAX）
	nextScnID_ = SCN_ID::TITLE;		// 予約（最初のシーン）

	charCnt_ = 0;

	AllScore_ = lpSaveLoad.ScoreCnt;
	
	mimiImage_[0] = LoadGraph(GH_MIMI_ISE);
	mimiImage_[1] = LoadGraph(GH_MIMI_IMIYA);
	mimiImage_[2] = LoadGraph(GH_MIMI_URAYAMA);
	mimiImage_[3] = LoadGraph(GH_MIMI_KAKITA);
	mimiImage_[4] = LoadGraph(GH_MIMI_KAZIWARA);
	himeiSE_ = LoadSoundMem(SH_HIMEI);

	//フォント画像読み込み
	constexpr int FontCntX = 10;
	constexpr int FontCntY = 1;
	LoadDivGraph(GH_LOGO_NUMBER_BW, FontCntX * FontCntY, FontCntX, FontCntY, FontNumSizeX, FontNumSizeY, NumberFontImage);

	return true;
}

//--------------------------------------------------------------------------
//●システムの初期化
//--------------------------------------------------------------------------
bool SceneCtl::SysInit(void)
{
	SetWindowIconID(103);
	SetGraphMode(scnArea_.x, scnArea_.y, 32);
	ChangeWindowMode(true);
	SetWindowText("earrape ～責めたいあの娘にこの愛試せ～");
	//SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	_dbgSetup(scnArea_.x, scnArea_.y, 255);
	//srand((unsigned int)time(nullptr));
	return true;
}

//--------------------------------------------------------------------------
//●リソースの解放
//--------------------------------------------------------------------------
bool SceneCtl::Release(void)
{
	for (int num = 0; num < 10; num++)
	{
		DeleteGraph(NumberFontImage[num]);
	}

	for (int num = 0; num < 6; num++)
	{
		DeleteGraph(mimiImage_[num]);
	}

	DxLib_End();	// DXライブラリ終了処理
	return true;
}

//--------------------------------------------------------------------------
//●キー情報取得
//--------------------------------------------------------------------------
void SceneCtl::GetKeyData(void)
{
	memcpy(keyBufOld_, keyBuf_, sizeof(keyBufOld_));	// 1フレーム前のキー取得
	GetHitKeyStateAll(keyBuf_);	// 現在のキー取得
}

//--------------------------------------------------------------------------
//●各シーンの初期化
//--------------------------------------------------------------------------
void SceneCtl::SetupScene(void)
{
	ReleaseScene();

	// 1フレーム後のシーンの初期化
	switch (nextScnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Init();
		break;
	case SCN_ID::SELECT:
		selectScene_.Init();
		break;
	case SCN_ID::GAME:
		gameScene_.Init();
		break;
	case SCN_ID::RESULT:
		resultScene_.Init();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Init();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Init();
		break;
	case SCN_ID::PERFORM:
		performScene_.Init();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Init();
		break;
	default:
		TRACE("未知のシーンID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
	scnID_ = nextScnID_;
}

//--------------------------------------------------------------------------
//●各シーンの解放
//--------------------------------------------------------------------------
void SceneCtl::ReleaseScene(void)
{
	switch (oldScnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Release();
		break;
	case SCN_ID::SELECT:
		selectScene_.Release();
		break;
	case SCN_ID::GAME:
		gameScene_.Release();
		break;
	case SCN_ID::RESULT:
		resultScene_.Release();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Release();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Release();
		break;
	case SCN_ID::PERFORM:
		performScene_.Release();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Release();
		break;
	default:
		TRACE("未知のシーンID\n");
		titleScene_.Init();
		break;
	}
}
