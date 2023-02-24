#include <DxLib.h>
#include "PauseScene.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Common/ResourceMng.h"
#include "../Application.h"
#include "OptionScene.h"
#include "../UI/UiManager.h"

// ちょうどいい感じに調整すること
constexpr Vector2I pauseWndSize{ 1280,720 };

PauseScene::PauseScene():
	BaseScene{ ScreenID::Pause,SceneID::Pause }
{
	isDrawBackScene_ = true;
	Load();
	SetFrontScene(std::make_unique<OptionScene>());
	AddLoadedFunc([this](auto& ctr) { uiMng_->Begin(); });
}

BaseScene::SceneUptr PauseScene::MakeGameFunc(SceneUptr own)
{
	return std::make_unique<TitleScene>();
}

void PauseScene::Update(float delta, Controller& controller)
{
	uiMng_->Update(delta, *this, *objMng_, controller);
	
}

void PauseScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	DrawBackScene();
	uiMng_->Draw();
	//menuUICtrl_->Draw();
	/*if (controller_->MousePress(InputID::Attack))
	{
		switch (menuUICtrl_->GetNowMenu())
		{
		case PAUSE::RESTART:
			ChangeSceneID(SceneID::Game);
			DrawString(0, 0, L"再開が押されたよ", 0xffffff);
			break;
		case PAUSE::SETTING:
			DrawString(0, 0, L"設定が押されたよ", 0xffffff);
			break;
		case PAUSE::END:
			DrawString(0, 0, L"終了が押されたよ", 0xffffff);
			lpApp.End();
			break;
		default:
			DrawString(0, 0, L"どこも押されてないよ", 0xffffff);
			break;
		}
	}*/
	
}

void PauseScene::Load(void)
{
	//menuUICtrl_ = std::make_unique<MenuUICtrl<PAUSE>>(*controller_,
	//	static_cast<int>(PAUSE::MAX));
	//// 再開
	//menuUICtrl_->AddMenu(
	//	PAUSE::RESTART,
	//	std::make_unique<MenuUI>(Vector2{ 640.0f,410.0f },
	//		"Resource/resource/modoru.png"));
	//// 設定
	//menuUICtrl_->AddMenu(
	//	PAUSE::SETTING,
	//	std::make_unique<MenuUI>(Vector2{ 640.0f,450.0f },
	//		"Resource/resource/settei.png"));
	//// 終了
	//menuUICtrl_->AddMenu(
	//	PAUSE::END,
	//	std::make_unique<MenuUI>(Vector2{ 640.0f,490.0f },
	//		"Resource/resource/syuryou.png"));
	uiMng_ = std::make_unique<UiManager>(SceneID::Pause);
}