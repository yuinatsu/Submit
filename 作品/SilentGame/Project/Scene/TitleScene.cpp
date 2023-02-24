#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "OptionScene.h"
#include "Transition/FadeTrans.h"
#include "../common/SoundPross.h"
#include "../common/ImageMng.h"
#include "../Input/InputConfig.h"
#include "LoadScene.h"
#include "../common/MenuUI/MenuUICtrl.h"
#include "../Scene/SceneMng.h"

#include "../common/Debug.h"

constexpr Math::Vector2I endWndSize{ 350,200 };
constexpr Math::Vector2I endWndPos{ 355,350 };

constexpr float endWndActTime{ 0.25f };

TitleScene::TitleScene()
{
	Init();
	//DrawOwnScreen(0.0);
}

TitleScene::~TitleScene()
{
	DeleteGraph(blendGraph_);
	DeleteGraph(blendNozeGraph_);
	DeleteGraph(endWndScreenH_);
	//lpImageMng.Relese("Keydata");
	//lpImageMng.Relese("PadPS");
	//lpImageMng.Relese("PadXbox");
	//lpImageMng.Relese("bg");
	//lpImageMng.Relese("buildings");
	//lpImageMng.Relese("far-buildings");
	//lpImageMng.Relese("skill-foreground");
	//lpImageMng.Relese("fogBG");
	//lpImageMng.Relese("fog");
	//lpImageMng.Relese("dummy");
	//lpImageMng.Relese("Sneaking_Mittion");
}

void TitleScene::SoundPlay(void)
{
	SoundProcess::SoundPlay(SoundProcess::SOUNDNAME_BGM::title, true, loopTime_);
	loopTime_ = 0.0f;
}

bool TitleScene::Init(void)
{
	// サウンドの初期化
	SoundProcess::Init();
	InitScreen();
	InitGame();
	return true;
}

void TitleScene::InitGame(void)
{
	// キー
	lpImageMng.GetID("./Resource/Input/Keydata.png", "Keydata", Vector2I(75, 25), Vector2I(10, 6));
	lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));

	// 背景
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/bg.png", "bg");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/buildings.png", "buildings");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/far-buildings.png", "far-buildings");
	lpImageMng.GetID("Resource/Image/TitleScene/Sprites/skill-foreground.png", "skill-foreground");
	lpImageMng.GetID("Resource/Image/fog/fogBG.png", "fogBG");
	lpImageMng.GetID("Resource/Image/fog/fog.png", "fog");

	// ブレンド画像
	blendGraph_ = LoadBlendGraph("Resource/Image/TitleScene/Blend_Cloud.png");
	blendNozeGraph_ = LoadBlendGraph("Resource/Image/TitleScene/Blend_Noise.png");
	lpImageMng.GetID("Resource/Image/TitleScene/dummy.png", "dummy");
	lpImageMng.GetID("Resource/Image/TitleScene/Sneaking_Mittion.png", "Sneaking_Mittion");

	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::title,SoundProcess::GetBGMVolumeEntire(), true);
	loopTime_ = 0.0f;
	updateFunc_ = &TitleScene::UpdateGame;
	drawFunc_ = &TitleScene::DrawGame;
}

void TitleScene::InitScreen(void)
{
	endWndScreenH_ = MakeScreen(endWndSize.x, endWndSize.y, true);
	menuUICtrl_ = std::make_unique<MenuUICtrl<TSELECT>>(*controller_, static_cast<int>(TSELECT::Max), 3.0f, TSELECT::Start);
	tstUICtrl_ = std::make_unique<MenuUICtrl<TST>>(*controller_, static_cast<int>(TST::MAX), 3.0f, TST::YES);

	menuUICtrl_->AddMenu(
		TSELECT::Start,
		std::make_unique<MenuUI>(
			Math::Vector2{ static_cast<float>(lpSceneMng.GetScreenSize().x / 2),400.0f },
			"Resource/Image/StringData/StringToGameStart.png"
			));

	menuUICtrl_->AddMenu(
		TSELECT::Setting,
		std::make_unique<MenuUI>(
			Math::Vector2{ static_cast<float>(lpSceneMng.GetScreenSize().x / 2),450.0f },
			"Resource/Image/StringData/StringToSetting.png"
			));

	menuUICtrl_->AddMenu(
		TSELECT::Suspension,
		std::make_unique<MenuUI>(
			Math::Vector2{ static_cast<float>(lpSceneMng.GetScreenSize().x / 2),500.0f },
			"Resource/Image/StringData/StringToGameEnd.png"
			));

	tstUICtrl_->SetMinusInputID(InputID::Left);
	tstUICtrl_->SetPlusInputID(InputID::Right);

	tstUICtrl_->AddMenu(
		TST::YES,
		std::make_unique<MenuUI>(
			Math::Vector2{ static_cast<float>(endWndSize.x) * 1.0f/4.0f, endWndSize.y - 25.0f},
			"Resource/Image/TitleScene/Yes.png"
			));

	tstUICtrl_->AddMenu(
		TST::NO,
		std::make_unique<MenuUI>(
			Math::Vector2{ static_cast<float>(endWndSize.x) * 3.0f / 4.0f,endWndSize.y - 25.0f },
			"Resource/Image/TitleScene/No.png"
			));
}

uniqueScene TitleScene::Update(float delta, uniqueScene ownScene)
{
	time_ += delta;
	return (this->*updateFunc_)(delta, std::move(ownScene));
}

uniqueScene TitleScene::UpdateGame(float delta, uniqueScene ownScene)
{
	loopTime_ += delta;
	// コントローラークラスの呼び出し
	controller_->Update(delta);
	// スキップ
	if (drawBlendTime_ < 256)
	{
		drawBlendTime_ += 5;
		if (controller_->Pressed(InputID::Attack))
		{
			drawBlendTime_ = 255;
		}
	}
	else if (drawBlendTime_ >= 256)
	{
		if (drawBlendNozeTime_ < 256)
		{
			drawBlendNozeTime_ += 5;
			if (controller_->Pressed(InputID::Attack))
			{
				drawBlendNozeTime_ = 255;
			}
		}
	}

	if (drawBlendTime_ >= 256 && drawBlendNozeTime_ >= 256)
	{
		menuUICtrl_->Update(delta);
		if (controller_->Pressed(InputID::Attack) && time_ >= 0.1)
		{
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);

			switch (menuUICtrl_->GetNowMenu())
			{
			case TSELECT::Start:
				SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::title);
				Relese();
				return std::make_unique<LoadScene>(
					1.0f,
					std::move(ownScene),
					std::make_unique<SelectScene>()
					);
				break;
			case TSELECT::Suspension:
				time_ = 0.0f;
				updateFunc_ = &TitleScene::UpdateOpenEnd;
				drawFunc_ = &TitleScene::DrawOpenEnd;;
				break;
			case TSELECT::Setting:
				SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::title);
				time_ = 0.0f;
				DxLib::SetDrawScreen(screenID_);
				DxLib::ClsDrawScreen();
				DrawBG();
				return std::make_unique<OptionScene>(std::move(ownScene));
				break;
			default:
				break;
			}
		}
	}
	DrawOwnScreen(delta);
	return ownScene;
}

uniqueScene TitleScene::UpdateEnd(float delta, uniqueScene ownScene)
{
	// コントローラークラスの呼び出し
	controller_->Update(delta);
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
			updateFunc_ = &TitleScene::UpdateCloseEnd;
			drawFunc_ = &TitleScene::DrawCloseEnd;
			break;
		default:
			break;
		}
	}
	DrawOwnScreen(delta);
	return ownScene;
}

uniqueScene TitleScene::UpdateOpenEnd(float delta, uniqueScene ownScene)
{
	if (time_ >= endWndActTime)
	{
		updateFunc_ = &TitleScene::UpdateEnd;
		drawFunc_ = &TitleScene::DrawEnd;
		time_ = 0.0f;
	}
	DrawOwnScreen(delta);
	return ownScene;
}

uniqueScene TitleScene::UpdateCloseEnd(float delta, uniqueScene ownScene)
{
	if (time_ >= endWndActTime)
	{
		updateFunc_ = &TitleScene::UpdateGame;
		drawFunc_ = &TitleScene::DrawGame;
		time_ = 0.0f;
	}
	DrawOwnScreen(delta);
	return ownScene;
}

void TitleScene::DrawLogoAndBg()
{
	// 文字列画像
	// タイトルロゴが表示されたら背景をBlendGraphで描画する
	if (drawBlendTime_ >= 256)
	{
		DrawBG();
		if (drawBlendNozeTime_ < 256)
		{
			DrawBlendGraph(0, 0, lpImageMng.GetID("dummy")[0], false, blendNozeGraph_, drawBlendNozeTime_, 60);
		}
	}

	constexpr int StringPos{ 500 };
	// 最初にタイトルロゴをBlendGraphで描画する
	DrawRotaGraph(StringPos + 25, 300, 1.0, 0.0, lpImageMng.GetID("Sneaking_Mittion")[0], true);
	if (drawBlendTime_ < 256)
	{
		DrawBlendGraph(0, 0, lpImageMng.GetID("dummy")[0], false, blendGraph_, drawBlendTime_, 60);
	}
}



void TitleScene::DrawOwnScreen(float delta)
{
	(this->*drawFunc_)(delta);
}

void TitleScene::DrawGame(float delta)
{
	// 自分自身のスクリーンに対してDraw
	DxLib::SetDrawScreen(screenID_);
	DxLib::ClsDrawScreen();
	DrawLogoAndBg();
	// 両方の表示が終わったら選択項目を表示する
	if (drawBlendTime_ >= 256 && drawBlendNozeTime_ >= 256)
	{
		menuUICtrl_->Draw();
		DrawKeyUD();
	}
}

void TitleScene::DrawEnd(float delta)
{
	DrawConfirmation();

	// 自分自身のスクリーンに対してDraw
	DxLib::SetDrawScreen(screenID_);
	DxLib::ClsDrawScreen();
	DrawLogoAndBg();
	DrawGraph(endWndPos.x, endWndPos.y, endWndScreenH_, true);
	DrawKeyLR();
}

void TitleScene::DrawOpenEnd(float delta)
{
	DrawConfirmation();

	float w{ time_ / endWndActTime };
	w = 1.0f - w;
	
	// 自分自身のスクリーンに対してDraw
	DxLib::SetDrawScreen(screenID_);
	DxLib::ClsDrawScreen();
	DrawLogoAndBg();
	DrawModiGraph(
		endWndPos.x, endWndPos.y + static_cast<int>(endWndSize.y / 2 * w),
		endWndPos.x + endWndSize.x, endWndPos.y + static_cast<int>(endWndSize.y / 2 * w),
		endWndPos.x + endWndSize.x, (endWndPos.y + endWndSize.y) - static_cast<int>(endWndSize.y/2 * w),
		endWndPos.x, (endWndPos.y + endWndSize.y) - static_cast<int>(endWndSize.y / 2 * w),
		endWndScreenH_, true);
	//DrawGraph(endWndPos.x, endWndPos.y, endWndScreenH_, true);
	DrawKeyLR();
}

void TitleScene::DrawCloseEnd(float delta)
{
	DrawConfirmation();

	float w{ time_ / endWndActTime };
	// 自分自身のスクリーンに対してDraw
	DxLib::SetDrawScreen(screenID_);
	DxLib::ClsDrawScreen();
	DrawLogoAndBg();
	DrawModiGraph(
		endWndPos.x, endWndPos.y + static_cast<int>(endWndSize.y / 2 * w),
		endWndPos.x + endWndSize.x, endWndPos.y + static_cast<int>(endWndSize.y / 2 * w),
		endWndPos.x + endWndSize.x, (endWndPos.y + endWndSize.y) - static_cast<int>(endWndSize.y / 2 * w),
		endWndPos.x, (endWndPos.y + endWndSize.y) - static_cast<int>(endWndSize.y / 2 * w),
		endWndScreenH_, true);
	DrawKeyLR();
}

// 背景の描画
void TitleScene::DrawBG(void)
{
	// 背景の描画
	DrawRotaGraph(550, 300, 4.5, 0.0, lpImageMng.GetID("bg")[0], true);
	DrawRotaGraph(far_buildingsPos_, 345, 4.5, 0.0, lpImageMng.GetID("far-buildings")[0], true);
	DrawRotaGraph(far_buildingsPos_ - 1200, 345, 4.5, 0.0, lpImageMng.GetID("far-buildings")[0], true);
	if (far_buildingsPos_ == 1580)	// スクロール
	{
		far_buildingsPos_ = far_buildingsPos_ - 1200;
	}
	far_buildingsPos_++;
	DrawRotaGraph(buildings_, 330, 4.5, 0.0, lpImageMng.GetID("buildings")[0], true);
	DrawRotaGraph(buildings_ - 1200, 330, 4.5, 0.0, lpImageMng.GetID("buildings")[0], true);
	if (buildings_ == 1670)
	{
		buildings_ = buildings_ - 1200;
	}
	buildings_ += 2;
	DrawRotaGraph(skill_foreground_, 430, 4.5, 0.0, lpImageMng.GetID("skill-foreground")[0], true);
	DrawRotaGraph(skill_foreground_ - 1200, 430, 4.5, 0.0, lpImageMng.GetID("skill-foreground")[0], true);
	if (skill_foreground_ == 1660)
	{
		skill_foreground_ = skill_foreground_ - 1200;
	}
	skill_foreground_ += 3;
	DrawGraph(0, 0, lpImageMng.GetID("fogBG")[0], true);
	//DrawGraph(0, 0, lpImageMng.GetID("fog")[0], true);
}

void TitleScene::DrawKeyUD(void)
{
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

void TitleScene::DrawKeyLR(void)
{
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	auto paddata = lpImageMng.GetID("PadXbox");
	if (type == 3 || type == 4)
	{
		paddata = lpImageMng.GetID("PadPS");
	}
	auto cntType = controller_->GetCntType();
	DrawGraph(700, 600, lpImageMng.GetID("./Resource/Image/Option/selectLR.png")[0], true);
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

void TitleScene::DrawConfirmation(void)
{
	DxLib::SetDrawScreen(endWndScreenH_);
	DxLib::ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("./Resource/Image/TitleScene/End.png")[0], true);
	tstUICtrl_->Draw();

}



void TitleScene::Relese(void)
{
	lpImageMng.Relese("bg");
	lpImageMng.Relese("buildings");
	lpImageMng.Relese("far-buildings");
	lpImageMng.Relese("skill-foreground");
	lpImageMng.Relese("fog");
	lpImageMng.Relese("fogBG");
	DeleteGraph(blendGraph_);
	DeleteGraph(blendNozeGraph_);
	lpImageMng.Relese("dummy");
	lpImageMng.Relese("Sneaking_Mittion");
	menuUICtrl_->Relese();
	DeleteGraph(endWndScreenH_);
}

