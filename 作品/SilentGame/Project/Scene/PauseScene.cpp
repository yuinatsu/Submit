#include <DxLib.h>
#include "GameScene.h"
#include "PauseScene.h"
#include "SelectScene.h"
#include "OptionScene.h"
#include "TutorialScene.h"
#include "LoadScene.h"
#include "../common/SoundPross.h"
#include "../common/ImageMng.h"
#include "SceneMng.h"
#include "../common/MenuUI/MenuUICtrl.h"
#include "../ItemDescription/ItemDescriptionWnd.h"
#include "../Input/InputConfig.h"
#include "Tutorial/PlayerTutorial.h"
#include "Tutorial/ItemTutorial.h"
#include "Tutorial/DoorTutorial.h"

constexpr Math::Vector2I pauseWndSize{ 780,340 };
constexpr Math::Vector2I pauseWndPos{ 140, 160 };
constexpr float wndActTime{ 0.25f };

PauseScene::PauseScene(uniqueScene beforScene, int mapNum , bool isSmallMap, Scene scene)
{
	mapNum_ = mapNum;
	isSmallMap_ = isSmallMap;
	oldScene_ = scene;
	beforScene_ = std::move(beforScene);
	Init();
	DrawOwnScreen(0.0);
}

PauseScene::~PauseScene()
{
	Relese();
}

bool PauseScene::Init(void)
{
	time_ = 0.0;
	SceneFlag_ = false;

	menuUICtrl_ = std::make_unique<MenuUICtrl<PSELECT>>(*controller_, static_cast<int>(PSELECT::Max), 2.0f , PSELECT::Resume);
	tstUICtrl_ = std::make_unique<MenuUICtrl<TST>>(*controller_, static_cast<int>(TST::MAX), 3.0f, TST::YES);

	menuUICtrl_->AddMenu(
		PSELECT::Resume,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),40.0f },
			"Resource/Image/Pause/saikai.png"
			));

	menuUICtrl_->AddMenu(
		PSELECT::Retry,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),90.0f },
			"Resource/Image/Pause/ritorai.png"
			));

	menuUICtrl_->AddMenu(
		PSELECT::SelectScene,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),140.0f },
			"Resource/Image/Pause/serekuto.png"
			));

	menuUICtrl_->AddMenu(
		PSELECT::Item,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),190.0f },
			"Resource/Image/Pause/Item.png"
			));

	menuUICtrl_->AddMenu(
		PSELECT::Setting,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),240.0f },
			"Resource/Image/Pause/settei.png"
			));

	menuUICtrl_->AddMenu(
		PSELECT::Suspension,
		std::make_unique<MenuUI>(Math::Vector2{ static_cast<float>(pauseWndSize.x / 2),290.0f },
			"Resource/Image/Pause/tyuudan.png"
			));

	tstUICtrl_->SetMinusInputID(InputID::Left);
	tstUICtrl_->SetPlusInputID(InputID::Right);

	tstUICtrl_->AddMenu(
		TST::YES,
		std::make_unique<MenuUI>(
			Math::Vector2{ 300.0f,220.0f },
			"Resource/Image/TitleScene/Yes.png"
			));

	tstUICtrl_->AddMenu(
		TST::NO,
		std::make_unique<MenuUI>(
			Math::Vector2{ 470.0f,220.0f },
			"Resource/Image/TitleScene/No.png"
			));

	// キー
	lpImageMng.GetID("./Resource/Input/Keydata.png", "Keydata", Vector2I(75, 25), Vector2I(10, 6));
	lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));
	lpImageMng.GetID("Resource/Image/Pause/PauseFrame.png", "PauseFrame.png");

	pauseWndScreenH_ = MakeScreen(pauseWndSize.x, pauseWndSize.y, true);

	updateFunc_ = &PauseScene::UpdateOpen;
	drawFunc_ = &PauseScene::DrawOpen;
	isNextOption_ = false;
	itemWnd_ = std::make_unique<ItemDescriptionWnd>(
		*controller_,
		Math::zeroVector2<float>,
		static_cast<Math::Vector2>(pauseWndSize)
	);
	return true;
}

uniqueScene PauseScene::Update(float delta, uniqueScene ownScene)
{
	controller_->Update(delta);
	time_ += delta;
	DrawOwnScreen(delta);
	//return (this->*updata_)(delta, std::move(ownScene));
	return (this->*updateFunc_)(delta, std::move(ownScene));
}

uniqueScene PauseScene::UpdateSetting(float delta, uniqueScene ownScene)
{
	if (time_ > 0.1)
	{
		menuUICtrl_->Update(delta);
		if (controller_->Pressed(InputID::Attack) && scene_ == 0)			// 決定キーが押された時の状態遷移
		{
			time_ = 0.0f;
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
			switch (menuUICtrl_->GetNowMenu())
			{
			case PSELECT::Resume:	// 中断中のゲームを再開
				isNextOption_ = false;
				updateFunc_ = &PauseScene::UpdateClose;
				drawFunc_ = &PauseScene::DrawClose;
				break;
			case PSELECT::Retry:	// 最初からプレイ
				if (oldScene_ == Scene::Game)
				{
					return std::make_unique<LoadScene>(1.0f, std::move(ownScene), std::make_unique<GameScene>(mapNum_, isSmallMap_));
				}
				else if (oldScene_ == Scene::Tutorial)
				{
					if (mapNum_ == 1)
					{
						return std::make_unique<LoadScene>(2.0f,std::move(ownScene),std::make_unique<PlayerTutorial>(mapNum_));
					}
					else if (mapNum_ == 2)
					{
						return std::make_unique<LoadScene>(2.0f,std::move(ownScene),std::make_unique<ItemTutorial>(mapNum_));
					}
					else if (mapNum_ == 3)
					{
						return std::make_unique<LoadScene>(2.0f,std::move(ownScene),std::make_unique<DoorTutorial>(mapNum_));
					}
				}
				break;
			case PSELECT::SelectScene:	// セレクトシーンに移行
				return std::make_unique<LoadScene>(1.0f, std::move(ownScene), std::make_unique<SelectScene>());
				break;
			case PSELECT::Item:
				updateFunc_ = &PauseScene::UpdateWnd;
				drawFunc_ = &PauseScene::DrawWnd;
				break;
			case PSELECT::Setting:	// 設定の画面に移行
				isNextOption_ = true;
				updateFunc_ = &PauseScene::UpdateClose;
				drawFunc_ = &PauseScene::DrawClose;
				break;
			case PSELECT::Suspension:	// ゲーム終了
				updateFunc_ = &PauseScene::UpdateEnd;
				drawFunc_ = &PauseScene::DrawEnd;
				break;
			default:
				break;
			}
		}
	}
	return ownScene;
}

uniqueScene PauseScene::UpdateEnd(float delta, uniqueScene ownScene)
{
	UpdateConfirmation(delta);
	return ownScene;
}

uniqueScene PauseScene::UpdateClose(float delta, uniqueScene ownScene)
{
	if (time_ >= wndActTime)
	{
		time_ = 0.0f;
		if (isNextOption_)
		{
			return std::make_unique<OptionScene>(std::move(beforScene_), std::move(ownScene), mapNum_, oldScene_);
		}
		else
		{
			beforScene_->SoundPlay();
			return std::move(beforScene_);
		}
		
	}
	return ownScene;
}

void PauseScene::UpdateConfirmation(float delta)
{
	tstUICtrl_->Update(delta);
	if (controller_->Pressed(InputID::Attack) && time_ >= 0.3)
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
		switch (tstUICtrl_->GetNowMenu())
		{
		case TST::YES:
			time_ = 0.0f;
			SceneFlag_ = true;
			break;
		case TST::NO:
			time_ = 0.0f;
			updateFunc_ = &PauseScene::UpdateSetting;
			drawFunc_ = &PauseScene::DrawSetting;
			break;
		default:
			break;
		}
	}
}

void PauseScene::DrawOwnScreen(float delta)
{
	(this->*drawFunc_)(0.0f);
	
}

void PauseScene::DrawSetting(float delta)
{
	SetDrawScreen(pauseWndScreenH_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("PauseFrame.png")[0], true);
	menuUICtrl_->Draw();
	DrawKeyUD();

	SetDrawScreen(screenID_);
	beforScene_->Draw(0.0f);
	DrawGraph(pauseWndPos.x, pauseWndPos.y, pauseWndScreenH_, true);

}

void PauseScene::DrawEnd(float delta)
{
	SetDrawScreen(pauseWndScreenH_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("PauseFrame.png")[0], true);
	DrawRotaGraph(390, 170, 1, 0, lpImageMng.GetID("./Resource/Image/TitleScene/End.png")[0], true);
	tstUICtrl_->Draw();
	DrawKeyLR();

	SetDrawScreen(screenID_);
	beforScene_->Draw(0.0f);
	DrawGraph(pauseWndPos.x, pauseWndPos.y, pauseWndScreenH_, true);
}

void PauseScene::DrawKeyUD(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(460, 315, lpImageMng.GetID("./Resource/Image/Option/select.png")[0], true);
	DrawGraph(635, 315, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(710, 315, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		DrawGraph(710, 315, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

void PauseScene::DrawKeyLR(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(460, 315, lpImageMng.GetID("./Resource/Image/Option/selectLR.png")[0], true);
	DrawGraph(635, 315, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(710, 315, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		DrawGraph(710, 315, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

void PauseScene::DrawItemKeyAll(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(360, 315, lpImageMng.GetID("./Resource/Image/Option/selectLR.png")[0], true);
	DrawGraph(510, 315, lpImageMng.GetID("./Resource/Image/Option/mo.png")[0], true);
	DrawGraph(635, 315, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(570, 313, keydata[lpConfigMng.GetKeyInputCode().at(InputID::btn1)], true);
		DrawGraph(710, 315, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		DrawGraph(570, 313, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::btn1)], true);
		DrawGraph(710, 315, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

void PauseScene::DrawItemKeyEnd(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(635, 315, lpImageMng.GetID("./Resource/Image/Option/mo.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(710, 315, keydata[lpConfigMng.GetKeyInputCode().at(InputID::btn1)], true);
	}
	else
	{
		DrawGraph(710, 315, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::btn1)], true);
	}
}

void PauseScene::Relese(void)
{
	DeleteGraph(pauseWndScreenH_);
}

uniqueScene PauseScene::UpdateWnd(float delta, uniqueScene ownScene)
{
	if (!itemWnd_->Update(delta))
	{
		updateFunc_ = &PauseScene::UpdateSetting;
		drawFunc_ = &PauseScene::DrawSetting;
	}
	return ownScene;
}

uniqueScene PauseScene::UpdateOpen(float delta, uniqueScene ownScene)
{
	if (time_ >= wndActTime)
	{
		time_ = 0.0f;
		updateFunc_ = &PauseScene::UpdateSetting;
		drawFunc_ = &PauseScene::DrawSetting;
	}
	return ownScene;
}

void PauseScene::DrawWnd(float delta)
{
	SetDrawScreen(pauseWndScreenH_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("PauseFrame.png")[0], true);
	itemWnd_->Draw();
	if (itemWnd_->GetFlag())
	{
		DrawItemKeyEnd();
	}
	else
	{
		DrawItemKeyAll();
	}

	SetDrawScreen(screenID_);
	beforScene_->Draw(0.0f);
	DrawGraph(pauseWndPos.x, pauseWndPos.y, pauseWndScreenH_, true);
	
}

void PauseScene::DrawOpen(float delta)
{
	SetDrawScreen(pauseWndScreenH_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("PauseFrame.png")[0], true);
	menuUICtrl_->Draw();
	DrawKeyUD();

	SetDrawScreen(screenID_);
	beforScene_->Draw(0.0f);

	float w{ time_ / wndActTime };
	w = 1.0f - w;
	DrawModiGraph(
		pauseWndPos.x, pauseWndPos.y + static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x + pauseWndSize.x, pauseWndPos.y + static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x + pauseWndSize.x, (pauseWndPos.y + pauseWndSize.y) - static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x, (pauseWndPos.y + pauseWndSize.y) - static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndScreenH_, true);
}

void PauseScene::DrawClose(float delta)
{
	SetDrawScreen(pauseWndScreenH_);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("PauseFrame.png")[0], true);
	menuUICtrl_->Draw();
	DrawKeyUD();

	SetDrawScreen(screenID_);
	beforScene_->Draw(0.0f);

	float w{ time_ / wndActTime };
	DrawModiGraph(
		pauseWndPos.x, pauseWndPos.y + static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x + pauseWndSize.x, pauseWndPos.y + static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x + pauseWndSize.x, (pauseWndPos.y + pauseWndSize.y) - static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndPos.x, (pauseWndPos.y + pauseWndSize.y) - static_cast<int>(pauseWndSize.y / 2 * w),
		pauseWndScreenH_, true);
}
