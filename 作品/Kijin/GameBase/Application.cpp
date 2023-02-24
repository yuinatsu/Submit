#include "Application.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Common/Debug.h"
#include "Common/ResourceMng.h"
#include "Common/ThreadPool.h"
#include "Common/Input/InputConfig/InputConfig.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Common/Input/InputType/Keyboard.h"
#include "Common/Input/InputType/Pad.h"

// ポーズ画面は確認のためだけ
#include "Scene/PauseScene.h"
#include "Scene/Transition/Loading.h"




Application Application::instance_;

int Application::Run(void)
{
	if (!isInit_ || !Init())
	{
		return -1;
	}
	while (!ProcessMessage() && !isEnd_)
	{
		// 更新処理
		timeClass_.DeltaTimeStart();
		controller_->Update(timeClass_.GetDeltaTime<float>());
		scene_ = scene_->Update(std::move(scene_), timeClass_.GetDeltaTime<float>(), *controller_);
		timeClass_.GameTimeEnd();

		// 描画処理
		scene_->DrawScene();

		// バックバッファに描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		scene_->Draw();
		DebugInfoDraw();
		ScreenFlip();
		timeClass_.DeltaTimeEnd();
	}
	scene_.reset();

	Effkseer_End();


	return 0;
}


ResourceMng& Application::GetResourceMng(void)
{
	return *resourceMng_;
}

TimeClass& Application::GetTimeClass(void)
{
	return timeClass_;
}

Random& Application::GetRandomClass(void)
{
	return random_;
}

ThreadPool& Application::GetThredPool(void)
{
	return *threadPool_;
}

Application::Application() :
	isInit_{false}
{
	isInit_ = SysInit();
}

Application::~Application()
{
	InputConfig::Destroy();
	
		DxLib_End();
}


bool Application::Init(void)
{
	bool rtn = Effekseer_Init(8000) != -1;

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetChangeScreenModeGraphicsSystemResetFlag(false);


	// あらかじめ作成スクリーンを作成しておく
	SharedRenderTargetHandle tmp;
	lpResourceMng.MakeRenderTarget(tmp,ScreenID::Loading, screenSize_<float>, true);
	lpResourceMng.MakeRenderTarget(tmp,ScreenID::Transition, screenSize_<float>, true);


	threadPool_->WaitAllTask();

	SetUseASyncLoadFlag(true);
	//scene_ = std::make_unique<Loading>(std::make_unique<GameScene>(), TransitionType::Fade, 1.0f);
	scene_ = std::make_unique<Loading>(std::make_unique<TitleScene>(), TransitionType::Fade, 1.0f);
	timeClass_.DeltaTimeEnd();
	timeClass_.DeltaTimeStart();
	timeClass_.GameTimeEnd();
	return rtn;
}

bool Application::SysInit(void)
{
	isEnd_ = false;
	// メモリーリークチェック用のやつ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	DebugStart(timeClass_);
	SetWindowText(TEXT("GameBase"));
	SetGraphMode(screenSize_<int>.x, screenSize_<int>.y, 32);
	Set3DSoundOneMetre(1.0f);	// 3D空間の1メートル相当の設定
	ChangeWindowMode(true);

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	bool rtn = DxLib_Init() != -1;
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	// dxアーカイブのキーをセット
	SetDXArchiveKeyString(L"data");

	// マウスカーソルの表示をオフに
	SetMouseDispFlag(false);

	// リソース管理クラスを作成
	resourceMng_ = std::make_unique<ResourceMng>();

	// スレッドプールを作成
	threadPool_ = std::make_unique<ThreadPool>();

	InputConfig::Create();

	if (InputConfig::GetInstance().GetNowType() != -1)
	{
		controller_ = std::make_unique<Pad>(InputConfig::GetInstance().GetNowType());
	}
	else
	{
		controller_ = std::make_unique<Keyboard>();
	}

	// デバッグのセットアップ
	DebugSetUp();
	return rtn;
}
