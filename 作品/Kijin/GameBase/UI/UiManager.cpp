#include <Dxlib.h>
#include "UiManager.h"
#include "../Object/ObjectManager.h"
#include "LoadIndicatorUi.h"
#include "../Component/Behavior/PlayerBehavior.h"
#include "ComboUi.h"
#include "Gage.h"
#include "Image.h"
#include "Button.h"
#include "Cursor.h"
#include "../Application.h"
#include "../Common/ResourceMng.h"
#include "Slider.h"
#include "../Common/Debug.h"

UiManager::UiManager(SceneID scene)
{
	InitUI(scene);
}

UiManager::UiManager(SceneID scene, ResultAttribute atr)
{
	result_ = atr;
	InitUI(scene);
}

void UiManager::CreateTitleSceneUI(Vector2 pos, float interval)
{
	uiList_.emplace_back(
		std::make_unique<Image>(L"Resource/resource/TitleLogoKijin.png",
			Vector2{ pos.x, pos.y }));

	uiList_.emplace_back(
		std::make_unique<Image>(L"Resource/resource/TitleKey.png",
			Vector2{ pos.x, pos.y + interval }));
}

void UiManager::CreateSelectSceneUI(Vector2 pos, float interval)
{
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Game); },
		L"Resource/resource/play.png", L"Resource/resource/PauseButtonFrame.png",
		Vector2{ pos.x,pos.y }
	));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Option); },
		L"Resource/resource/option.png", L"Resource/resource/PauseButtonFrame.png",
		Vector2{ pos.x,pos.y + interval }));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { lpApp.End(); },
		L"Resource/resource/finish.png", L"Resource/resource/PauseButtonFrame.png",
		Vector2{ pos.x,pos.y + interval * 2 }
	));

	uiList_.emplace_back(std::make_unique<Slider>(
		[](const float v) {DebugLog(v); },
		"Resource/resource/Ui/SliderBar.png", "Resource/resource/Ui/SliderTab.png",
		0.0f, Vector2{ 100.0f, 100.0f }
	));

	uiList_.emplace_back(std::make_unique<Cursor>());
}

void UiManager::CreateGameSceneUI(Vector2 gaugePos, float interval, Vector2 comboPos)
{
	// アイコン座標
	Vector2 iconPos = { gaugePos.x - 15.0f,gaugePos.y + 10.0f};

	uiList_.emplace_back(
		std::make_unique<Gage>(
			[](ObjectManager& objMng) {
				auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
				auto [value, minmax] = player->GetGaugeValue(UiID::Hp);
				return value;
			},
			L"Resource/resource/Ui/Hp.png",
				L"Resource/resource/Ui/frame.png",
				1.0f, gaugePos
				));

	uiList_.emplace_back(
		std::make_unique<Gage>(
			[](ObjectManager& objMng) {
				auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
				auto [value, minmax] = player->GetGaugeValue(UiID::Skill);
				return value;
			},
			L"Resource/resource/Ui/Skill.png",
				L"Resource/resource/Ui/frame.png",
				0.0f, Vector2{ gaugePos.x,gaugePos.y + interval * 1 }
			));
	uiList_.emplace_back(
		std::make_unique<Gage>(
			[](ObjectManager& objMng) {
				auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
				auto [value, minmax] = player->GetGaugeValue(UiID::Stamina);
				return value;
			},
			L"Resource/resource/Ui/Stamina.png",
				L"Resource/resource/Ui/frame.png",
				1.0f, Vector2{ gaugePos.x,gaugePos.y + interval * 2 }
			));

	uiList_.emplace_back(
		std::make_unique<Image>(L"Resource/resource/Ui/magicCircle.png",
			Vector2{ iconPos.x,iconPos.y + interval * 1 }));
	uiList_.emplace_back(
		std::make_unique<Image>(L"Resource/resource/Ui/shoes.png",
			Vector2{ iconPos.x,iconPos.y + interval * 2 }));
	uiList_.emplace_back(
		std::make_unique<Image>(L"Resource/resource/Ui/heart.png",
			iconPos));
	uiList_.emplace_back(
		std::make_unique<ComboUi>(L"Resource/resource/Ui/combo.png",
			Vector2{ comboPos.x,comboPos.y }));
}

void UiManager::CreateResultSceneUi(Vector2 pos, float interval)
{
	if (result_ == ResultAttribute::Clear)
	{
		uiList_.emplace_back(
			std::make_unique<Image>(L"Resource/resource/GameClearLogo.png",
				Vector2{ pos.x, pos.y - 100.0f }));
	}
	else if(result_ == ResultAttribute::GameOver)
	{
		uiList_.emplace_back(
			std::make_unique<Image>(L"Resource/resource/GameOverLogo.png",
				Vector2{ pos.x, pos.y - 100.0f }));
	}

	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Game); },
		L"Resource/resource/retry.png", L"Resource/resource/RetryFrame.png",
		Vector2{ pos.x,pos.y }
	));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Select); },
		L"Resource/resource/stageSelect.png", L"Resource/resource/StageSelectFrame.png",
		Vector2{ pos.x,pos.y + interval }
	));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { lpApp.End(); },
		L"Resource/resource/finish.png", L"Resource/resource/PauseButtonFrame.png",
		Vector2{ pos.x,pos.y + interval * 2 }
	));

	uiList_.emplace_back(std::make_unique<Cursor>());
}

void UiManager::CreateLoadSceneUI(Vector2 pos)
{
	uiList_.emplace_back(std::make_unique<LoadIndicatorUi>(L"Resource/resource/Ui/loading.png", Vector2{ pos.x,pos.y }));
}

void UiManager::CreatePauseSceneUI(Vector2 pos, float interval)
{
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeBackScene(); },
			L"Resource/resource/modoru.png", L"Resource/resource/PauseButtonFrame.png",
			Vector2{ pos.x,pos.y }
	));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Option); },
		L"Resource/resource/settei.png", L"Resource/resource/PauseButtonFrame.png",
	Vector2{ pos.x,pos.y + interval }	));
	uiList_.emplace_back(std::make_unique<Button>(
		[](BaseScene& scene) { lpApp.End(); },
		L"Resource/resource/syuryou.png", L"Resource/resource/PauseButtonFrame.png",
		Vector2{ pos.x,pos.y + interval * 2 }
	));
	uiList_.emplace_back(std::make_unique<Cursor>());
}

void UiManager::CreateOptionSceneUI(void)
{
}

UiManager::~UiManager()
{
	uiList_.clear();
}

void UiManager::InitUI(SceneID scene)
{
	scene_ = scene;
	auto screensize(lpApp.screenSize_<float>);
	switch (scene_)
	{
	case SceneID::Title:
		CreateTitleSceneUI(Vector2{ screensize.x / 2.0f, 300.0f }, 200.0f);
		break;
	case SceneID::Select:
		CreateSelectSceneUI(screensize / 2.0f, 60.0f);
		break;
	case SceneID::Game:
		CreateGameSceneUI(Vector2{ 50.0f,650.0f }, 23.0f, Vector2{ 1000,50 });
		break;
	case SceneID::Result:
		CreateResultSceneUi(Vector2{ screensize.x / 2.0f, 410.0f }, 40.0f);
		break;
	case SceneID::Loading:
		CreateLoadSceneUI(screensize / 2.0f);
		break;
	case SceneID::Pause:
		CreatePauseSceneUI(Vector2{ screensize.x / 2.0f, 410.0f }, 40.0f);
		break;
	case SceneID::Option:
		CreateOptionSceneUI();
		break;
	}
}

void UiManager::Begin()
{
	for (auto& ui : uiList_)
	{
		ui->Loaded();
	}

}

void UiManager::Update(float delta, BaseScene& scene, ObjectManager& obj, Controller& controller)
{
	if (scene_ == SceneID::Game)
	{
		auto player = obj.GetComponent<PlayerBehavior>(obj.GetPlayerID());
		if (!player.IsActive())
		{
			uiList_.clear();
		}
	}

	for (auto& ui : uiList_)
	{
		ui->Update(delta, obj, controller);
	}

	auto cursor = std::find_if(uiList_.begin(), uiList_.end(), [](std::unique_ptr<UiBase>& ui) { return ui->GetUIID() == UiID::Cursor; });
	if (cursor != uiList_.end())
	{
		static_cast<Cursor*>((*cursor).get())->Check(uiList_, scene,controller);
	}
}

void UiManager::Draw(void)
{
	for (auto& ui : uiList_)
	{
		ui->Draw();
	}
}
