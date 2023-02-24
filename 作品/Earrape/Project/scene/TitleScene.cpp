#include <DxLib.h>
#include "TitleScene.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "SceneCtl.h"
#include "../input/Mouse.h"
#include "../_debug/_DebugConOut.h"

//--------------------------------------------------------------------------
//●シーンの更新
//	keyData：入力した最新のキー情報
//	keyDataOld：keyDataの１つ前のキー情報
//--------------------------------------------------------------------------
SCN_ID TitleScene::Update(char* keyData, char* keyDataOld)
{
	//SetBackgroundColor(0, 0, 0);
	lpMouse.Update();
	const auto inputData = lpMouse.GetData(InputType::NOW);
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);
	pos_.x = inputData.at("X座標");
	pos_.y = inputData.at("Y座標");

	sctFlag1_ = false;
	sctFlag2_ = false;
	sctFlag3_ = false;

	//TRACE("x:%d  y:%d\n", pos_.x, pos_.y);//マウス座標デバッグ表示

	if (inputData.count("X座標") && inputData.count("Y座標"))
	{
		//項目１：GameStart
		auto sctPosA = (lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2;
		auto sctPosB = (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2;
		if ((pos_.x >= sctPosA) && (pos_.y >= sctPosB) &&
			(pos_.x <= sctPosA + lpSize.titleSect1_.x) && (pos_.y <= sctPosB + lpSize.titleSect1_.y))
		{
			sctFlag1_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleSceneからSelectSceneに移行\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//音量設定
				PlaySoundMem(atfSE_, DX_PLAYTYPE_NORMAL, true);
				//lpGameScene.DataLoad();// データロード//いったん消してる
				return SCN_ID::SELECT;
			}
		}
		//項目２：PlayGuide
		auto sctPosC = (lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2;
		auto sctPosD = (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2;
		if ((pos_.x >= sctPosC) && (pos_.y >= sctPosD) &&
			(pos_.x <= sctPosC + lpSize.titleSect2_.x) && (pos_.y <= sctPosD + lpSize.titleSect2_.y))
		{
			sctFlag2_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleSceneからGuideSceneに移行\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//音量設定
				PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE2), DX_PLAYTYPE_BACK, true);
				return SCN_ID::GUIDE;
			}
		}
		//項目３：Databese
		auto sctPosE = (lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 2;
		auto sctPosF = (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 2;
		if ((pos_.x >= sctPosE) && (pos_.y >= sctPosF) &&
			(pos_.x <= sctPosE + lpSize.titleSect3_.x) && (pos_.y <= sctPosF + lpSize.titleSect3_.y))
		{
			sctFlag3_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleSceneからDataBaseSceneに移行\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//音量設定
				PlaySoundMem(atfSE_, DX_PLAYTYPE_BACK, true);
				return SCN_ID::DATABASE;
			}
		}
	}
	return SCN_ID::TITLE;
}

//--------------------------------------------------------------------------
//●グラフィックの描画
//--------------------------------------------------------------------------
void TitleScene::Draw(void)
{
	//TRACE("TitleSceneの描画\n");
	DrawGraph(0, 0, titlebg_, false);

	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleLogo_.x) / 2, (lpSceneCtl.scnArea_.y - lpSize.titleLogo_.y) / 4, titleLogoBW_, true);

	if (!sctFlag1_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2, section1_BW_, true);
	}
	if (!sctFlag2_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2, section2_BW_, true);
	}
	if (!sctFlag3_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 2, section3_BW_, true);
	}


	if (sctFlag1_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2, section1_BO_, true);
	}
	if (sctFlag2_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2, section2_BO_, true);
	}
	if (sctFlag3_)
	{
		DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 2, section3_BO_, true);
	}
}

//--------------------------------------------------------------------------
//●初期化
//--------------------------------------------------------------------------
bool TitleScene::Init(void)
{
	//TRACE("TitleSceneの初期化\n");
	bool rtnFlag = true;
	//GetDrawScreenSize(&scrSize_.x, &scrSize_.y);

	titleLogoBO_ = LoadGraph(GH_LOGO_TITLE_BO);
	titleLogoBW_ = LoadGraph(GH_LOGO_TITLE_BW);
	section1_BO_ = LoadGraph(GH_LOGO_TITLE_SECTION_1_BO);
	section1_BW_ = LoadGraph(GH_LOGO_TITLE_SECTION_1_BW);
	section2_BO_ = LoadGraph(GH_LOGO_TITLE_SECTION_2_BO);
	section2_BW_ = LoadGraph(GH_LOGO_TITLE_SECTION_2_BW);
	section3_BO_ = LoadGraph(GH_LOGO_TITLE_SECTION_3_BO);
	section3_BW_ = LoadGraph(GH_LOGO_TITLE_SECTION_3_BW);
	titlebg_ = LoadGraph(GH_BG_TITLE);
	atfSE_ = LoadSoundMem(SH_ATF);
	okSE_ = LoadSoundMem(SH_OK);

	return true;
}

//--------------------------------------------------------------------------
//●リソースの解放
//--------------------------------------------------------------------------
bool TitleScene::Release(void)
{
	TRACE("TitleSceneの解放\n");
	DeleteGraph(titleLogoBO_);
	DeleteGraph(titleLogoBW_);
	DeleteGraph(titleGuideLogoBO_);
	DeleteGraph(titleGuideLogoBW_);
	DeleteGraph(atfSE_);
	DeleteGraph(okSE_);
	return true;
}
