#include <DxLib.h>
#include "TitleScene.h"
#include "SceneMng.h"
#include "SelectScene.h"
#include "../common/ImageMng.h"
#include "Transition/CrossOver.h"
#include "../../_debug/_DebugConOut.h"
#include "../common/FilePass.h"
#include "../common/FileSize.h"


TitleScene::TitleScene()
{
	Init();
	DrawOwnScreen(0.0f);
}


TitleScene::~TitleScene()
{
}


bool TitleScene::Init(void)
{
	lpImageMng.GetID(GH_BG_TITLE,"title");
	lpImageMng.GetID(GH_LOGO_TITLE, "logo");
	lpImageMng.GetID(GH_LOGO_TITLE_FRAME, "frame");
	lpImageMng.GetID(GH_LOGO_SELECT_KEY, "key");
	titlePos_ = Vector2F((lpSceneMng.GetScreenSize().x - lpSize.titleLogoSize_.x) / 2, -lpSize.titleLogoSize_.y);
	titleFlag_ = false;
	logoFlag_ = true;
	count_ = 0;
	titleBGM_ = LoadSoundMem(SH_BGM_TITLE);
	titleSE_ = LoadSoundMem(SH_SE_TITLE);
	PlaySoundMem(titleBGM_, DX_PLAYTYPE_LOOP, true);
	return true;
}


uniqueBaseScene TitleScene::Update(double delta, uniqueBaseScene own)
{
	if (titleFlag_ && count_ < 2.0)
	{
		count_ += delta;
		if (count_ >= 2.0)
		{
			logoFlag_ = false;
		}
	}

	if ((lpSceneMng.GetScreenSize().y - lpSize.titleLogoSize_.y) / 4 >= titlePos_.y)
	{
		titlePos_.y += 2;
	}
	else
	{
		titleFlag_ = true;
	}

	if (!logoFlag_ && (lpSceneMng.keyBuf_[KEY_INPUT_Z] && !lpSceneMng.keyBufOld_[KEY_INPUT_Z]))
	{
		PlaySoundMem(titleSE_, DX_PLAYTYPE_BACK, true);
		StopSoundMem(titleBGM_);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<SelectScene>());
	}
	DrawOwnScreen(delta);
	return std::move(own);
}


void TitleScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClearDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("title")[0], true);
	DrawGraph(titlePos_.x, titlePos_.y, lpImageMng.GetID("logo")[0], true);
	if (titleFlag_)
	{
		PlayMovieToGraph(lpImageMng.GetID("key")[0]);
		auto alpha = static_cast<int>(255.0 * count_ / 2.0);			// 255(α値のMAX) : 3.0(クロスオーバーの時間) = alpha(現在時間のα値) : count(処理開始からの経過時間)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(((lpSceneMng.GetScreenSize().x - lpSize.titleKeySize_.x) / 2) - 10, ((lpSceneMng.GetScreenSize().y - lpSize.titleKeySize_.y) / 3 * 2) - 10, lpImageMng.GetID("frame")[0], true);
		DrawGraph((lpSceneMng.GetScreenSize().x - lpSize.titleKeySize_.x) / 2, (lpSceneMng.GetScreenSize().y - lpSize.titleKeySize_.y) / 3 * 2, lpImageMng.GetID("key")[0], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
		
	}
}
