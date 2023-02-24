#include <DxLib.h>
#include "ResultScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "../common/ResultData.h"
#include "../common/SoundPross.h"
#include "../common/ResultDraw/ResultDrawCtrl.h"
#include "../common/MenuUI/MenuUICtrl.h"
#include "../Scene/SceneMng.h"
#include "../common/ImageMng.h"
#include "../Input/InputConfig.h"

#include "../common/Debug.h"

ResultScene::ResultScene(ResultData& result,int mapNum, bool isSmallMap,float delta)
{
	mapNum_ = mapNum;
	isSmallMap_ = isSmallMap;
	resultDrawCtrl_ = std::make_unique<ResultDrawCtrl>(result,delta);
	Init();

	DebugLog("アイテム使用回数:", result.GetItemUseCount());
	DrawOwnScreen(0.0);
}


ResultScene::~ResultScene()
{
}

bool ResultScene::Init(void)
{
	InitScreen();
	lpImageMng.GetID("Resource/Image/ResultScene/Retry.png", "Retry");
	lpImageMng.GetID("Resource/Image/ResultScene/SceneSelect.png", "SceneSelect");
	lpImageMng.GetID("Resource/Image/ResultScene/Title.png", "Title");
	// キー
	lpImageMng.GetID("./Resource/Input/Keydata.png", "Keydata", Vector2I(75, 25), Vector2I(10, 6));
	lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));

	menuUICtrl_ = std::make_unique<MenuUICtrl<RSELECT>>(*controller_, static_cast<int>(TSELECT::Max), 3.0f,RSELECT::Retry);

	// リトライメニューを追加
	menuUICtrl_->AddMenu(
		RSELECT::Retry,
		std::make_unique<MenuUI>(
			Math::Vector2{static_cast<float>(lpSceneMng.GetScreenSize().x / 2),450.0f},
			"Retry"
		));

	// セレクトへ行くメニューを追加
	menuUICtrl_->AddMenu(
		RSELECT::Select,
		std::make_unique<MenuUI>(
			Math::Vector2{static_cast<float>(lpSceneMng.GetScreenSize().x / 2),510.0f},
			"SceneSelect"
		));

	// タイトルへ戻るメニューを追加
	menuUICtrl_->AddMenu(
		RSELECT::Title,
		std::make_unique<MenuUI>(
			Math::Vector2{static_cast<float>(lpSceneMng.GetScreenSize().x / 2),570.0f},
			"Resource/Image/ResultScene/Title.png"
		));
	return true;
}

void ResultScene::InitScreen(void)
{
	SceneFlag_ = false;
}

uniqueScene ResultScene::Update(float delta, uniqueScene ownScene)
{
	DrawOwnScreen(delta);
	controller_->Update(delta);
	return UpdateSelect(delta, std::move(ownScene));
}

void ResultScene::Relese(void)
{
	menuUICtrl_->Relese();
}

uniqueScene ResultScene::UpdateSelect(float delta, uniqueScene ownScene)
{
	resultDrawCtrl_->Update(delta);
	if (resultDrawCtrl_->IsStartedDraw())
	{
		
		menuUICtrl_->Update(delta);
		// シーン移行する
		if (controller_->Pressed(InputID::Attack))
		{
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
			Relese();
			switch (menuUICtrl_->GetNowMenu())
			{
			case RSELECT::Retry:
				return std::make_unique<LoadScene>(1.0f, std::move(ownScene), std::make_unique<GameScene>(mapNum_, isSmallMap_));
				break;
			case RSELECT::Select:
				return std::make_unique<SelectScene>();
				break;
			case RSELECT::Title:
				return std::make_unique<TitleScene>();
				break;
			default:
				break;
			}
		}
	}
	return ownScene;
}

void ResultScene::DrawOwnScreen(float delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	SetDrawBright(255, 255, 255);

	resultDrawCtrl_->Draw();

	if (resultDrawCtrl_->IsStartedDraw())
	{
		count_ += delta;
		if (count_ > 0.5f)
		{
			menuUICtrl_->Draw();
		}
	}
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(700, 600, lpImageMng.GetID("./Resource/Image/Option/select.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(875, 600, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(950, 600, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		//DrawFormatString(775, 425, 0xffffff, "戻る：");
		DrawGraph(875, 600, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(950, 600, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

