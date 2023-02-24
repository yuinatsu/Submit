#include <cassert>
#include <DxLib.h>

#include "SceneMng.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Map/MapData.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"


bool SceneMng::SysInit(void)
{
	screenSize_ = { 1024,768 };
	SetOutApplicationLogValidFlag(false);
	SetAlwaysRunFlag(true);
	SetGraphMode(screenSize_.x, screenSize_.y, 32);
	ChangeWindowMode(true);
	SetWindowText("2016010_äÅå¥èÆêl");

	if (DxLib_Init() == -1)
	{
		//assert(!"DxLibInité∏îs");
		return false;
	}

	return true;
}

void SceneMng::Run(void)
{
	if (!SysInit())
	{
		return;
	}
	scene_ = std::make_unique<TitleScene>();
	now_ = std::chrono::system_clock().now();

	_dbgSetup(screenSize_.x, screenSize_.y, 255);

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		_dbgStartDraw();
		old_ = now_;
		now_ = std::chrono::system_clock().now();

		double delta = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(now_ - old_).count() / 1000000.0);

		scene_ = scene_->Update(delta, std::move(scene_));

		scene_->DrawOwnScreen(delta);

		SetDrawScreen(DX_SCREEN_BACK);
		

		scene_->Draw(delta);

		// deltaTimeï\é¶
		//DrawBox(0, 0, 80, 16, 0x000000, true);
		//DrawFormatString(0, 0, 0xffffff, "%lf\n", delta);

		_dbgDraw();
		ScreenFlip();
	}
	DxLib_End();
}

const Vector2& SceneMng::GetScreenSize(void)
{
	return screenSize_;
}

SceneMng::SceneMng()
{
}

SceneMng::~SceneMng()
{
}
