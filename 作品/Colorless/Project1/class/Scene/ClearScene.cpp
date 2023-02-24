#include <DxLib.h>
#include "ClearScene.h"
#include "SceneMng.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "../common/ImageMng.h"
#include "Transition/CrossOver.h"
#include "../common/FilePass.h"
#include "../common/FileSize.h"

ClearScene::ClearScene()
{
	Init();
	DrawOwnScreen(0.0f);
}

ClearScene::~ClearScene()
{
}

bool ClearScene::Init(void)
{
	lpImageMng.GetID(GH_BG_CLEAR, "bg");
	lpImageMng.GetID(GH_LOGO_CLEAR, "clear");
	lpImageMng.GetID(GH_LOGO_MENU, "menu", lpSize.menuSize_, lpSize.menuDiv_);
	lpImageMng.GetID(GH_ICON_ITEM, "icon", Vector2(lpSize.blockSize_.x, lpSize.blockSize_.y), Vector2(lpSize.blockDiv_.x - 2, lpSize.blockDiv_.y));
	cursorPos_ = Vector2F((lpSceneMng.GetScreenSize().x - lpSize.menuImageSize_.x * 1.5) / 2 - 64, lpSceneMng.GetScreenSize().y / 3 + 16);
	cursorNum_ = 1;
	shadowPos_ = Vector2F(0, 80);
	blinkCnt_ = 0;
	clearSE_ = LoadSoundMem(SH_SE_CLEAR);
	return true;
}

uniqueBaseScene ClearScene::Update(double delta, uniqueBaseScene own)
{
	//if ((lpSceneMng.keyBuf_[KEY_INPUT_Z] && !lpSceneMng.keyBufOld_[KEY_INPUT_Z]))
	//{
	//	PlaySoundMem(clearSE_, DX_PLAYTYPE_BACK);
	//	return std::make_unique<CrossOver>(std::move(own), std::make_unique<SelectScene>());
	//}

	if (cursorNum_ <= 0 && (lpSceneMng.keyBuf_[KEY_INPUT_UP] && !lpSceneMng.keyBufOld_[KEY_INPUT_UP]))
	{
		cursorPos_.y -= 120;
		cursorNum_++;
	}
	if (cursorNum_ >= 1 && (lpSceneMng.keyBuf_[KEY_INPUT_DOWN] && !lpSceneMng.keyBufOld_[KEY_INPUT_DOWN]))
	{
		cursorPos_.y += 120;
		cursorNum_--;
	}

	if (shadowPos_.x <= 600)
	{
		shadowPos_.x += 10;
	}
	else
	{
		if ((lpSceneMng.keyBuf_[KEY_INPUT_Z] && !lpSceneMng.keyBufOld_[KEY_INPUT_Z]))
		{
			if (cursorNum_ == 1)
			{
				PlaySoundMem(clearSE_, DX_PLAYTYPE_BACK, true);
				return std::make_unique<CrossOver>(std::move(own), std::make_unique<SelectScene>());
			}
			else
			{
				PlaySoundMem(clearSE_, DX_PLAYTYPE_BACK, true);
				return std::make_unique<CrossOver>(std::move(own), std::make_unique<TitleScene>());
			}
		}
	}
	DrawOwnScreen(delta);
	blinkCnt_++;
	return std::move(own);
}

void ClearScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClearDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("bg")[0], true);
	DrawGraph(0, 80, lpImageMng.GetID("clear")[0], true);
	DrawBox(shadowPos_.x, shadowPos_.y, 500, shadowPos_.y + 80, 0x000000, true);
	DrawGraph((lpSceneMng.GetScreenSize().x - lpSize.menuImageSize_.x * 1.5) / 2, lpSceneMng.GetScreenSize().y / 3, lpImageMng.GetID("menu")[0], true);
	DrawGraph((lpSceneMng.GetScreenSize().x - lpSize.menuImageSize_.x * 1.5) / 2, lpSceneMng.GetScreenSize().y / 2, lpImageMng.GetID("menu")[1], true);
	if (blinkCnt_ <= 150 || blinkCnt_ / 30 % 2 != 0)
	{
		DrawGraph(cursorPos_.x, cursorPos_.y, lpImageMng.GetID("icon")[0], true);
	}
	DrawBox(shadowPos_.x, lpSceneMng.GetScreenSize().y / 3, 600, lpSceneMng.GetScreenSize().y / 2 + 60, 0x000000, true);
}
