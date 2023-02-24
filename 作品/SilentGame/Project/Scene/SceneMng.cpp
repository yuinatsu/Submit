#include "SceneMng.h"
#include <DxLib.h>
#include <memory>
#include <chrono>
#include "../common/Debug.h"
#include "../common/ImageMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "SelectScene.h"
#include "../Input/InputConfig.h"

using namespace std;

constexpr Math::Vector2I screenSize{ 1060,660 };

constexpr Math::Vector2I cameraSize{ 960,540 };

void SceneMng::Run(void)
{
	if (!InitFlag_)
	{
		// 確認のためもう一度イニシャライズする
		if (!SysInit())
		{
			return;
		}
	}
	CreateMaskScreen();
	scene_ = std::make_unique<TitleScene>();
	//scene_ = std::make_unique<SelectScene>();
	//scene_ = std::make_unique<LoadScene>(1.0f,std::make_unique<SelectScene>(),std::make_unique<GameScene>());
	//scene_ = std::make_unique<GameScene>();

	time_.DeltaTimeStart();
	time_.DeltaTimeEnd();
	time_.GameTimeEnd();
	auto flag = scene_->GetFlag();
	while (ProcessMessage() == 0 && !flag)
	{
		time_.DeltaTimeStart();
		scene_ = scene_->Update(time_.GetDeltaTime<float>(), std::move(scene_));
		time_.GameTimeEnd();

		Draw(time_.GetDeltaTime<float>());

		flag = scene_->GetFlag();
		time_.DeltaTimeEnd();
	}
	DeleteMaskScreen();
	InitGraph();
	InputConfig::Destroy();
	DxLib_End();
}

const Math::Vector2I& SceneMng::GetScreenSize(void) const
{
	return screenSize;
}

const Math::Vector2I& SceneMng::GetCameraSize(void) const
{
	return cameraSize;
}

bool SceneMng::SysInit(void)
{
	DebugStart(time_);
	SetWindowText("Sneaking Mission");
	SetGraphMode(screenSize.x, screenSize.y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	InputConfig::Create();
	DebugSetUp();
	return true;
}

void SceneMng::Update(void)
{
}

void SceneMng::Draw(float delta)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw(delta);
	DebugInfoDraw();
	ScreenFlip();
}

SceneMng::SceneMng()
{
	InitFlag_ = SysInit();
}

SceneMng::~SceneMng()
{
}
