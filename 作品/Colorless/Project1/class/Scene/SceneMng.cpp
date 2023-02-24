#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "../../_debug/_DebugDispOut.h"


void SceneMng::Run(void)
{
	// 正常にSysInitが呼び出されなかったら異常終了
	if (!initFlag_)
	{
		// 念のためもう一回だけ呼び出す
		if (!SysInit())
		{
			// 異常終了
			return;
		}
	}
	// Sceneのインスタンス
	scene_ = std::make_unique<TitleScene>();		
	// 初期のステージ情報格納
	stage_ = Stage::Stage1;
	lpColorMng.Init();

	// deltaTime
	std::chrono::system_clock::time_point nowTime, oldTime;
	nowTime = std::chrono::system_clock::now();
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgAddDraw();
		// deltaTimeの設定
		oldTime = nowTime;
		nowTime = std::chrono::system_clock::now();
		auto elTime = nowTime - oldTime;

		auto msec = std::chrono::duration_cast<std::chrono::microseconds>(elTime).count();
		auto delta = msec / 1000000.0;

		memcpy(keyBufOld_, keyBuf_, sizeof(keyBufOld_));			// keyBufをkeyBufOldにコピー
		GetHitKeyStateAll(keyBuf_);									// 全てのキーの状態をkeyBufに格納

		scene_ = scene_->Update(delta, std::move(scene_));

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		scene_->Draw(delta);
	
		ScreenFlip();
	}
}


void SceneMng::Update(void)
{
}


void SceneMng::Draw(void)
{
}


const Vector2 SceneMng::GetScreenSize(void) const
{
	return screenSize_;
}

Stage SceneMng::GetStage(void)
{
	return stage_;
}

void SceneMng::SetStage(int num)
{
	stage_ = static_cast<Stage>(num - 1);
}

CntType SceneMng::GetCntType(void)
{
	return cntType_;
}

void SceneMng::SetCntType(CntType cntType)
{
	cntType_ = cntType;
}

SceneMng::SceneMng():screenSize_(1024,768)
{
	initFlag_ = SysInit();
}


SceneMng::~SceneMng()
{
}


bool SceneMng::SysInit(void)
{
	SetWindowText("Colorless");
	SetGraphMode(screenSize_.x,screenSize_.y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) 
	{
		return false;
	}
	_dbgSetup(screenSize_.x, screenSize_.y, 255);
	return true;
}
