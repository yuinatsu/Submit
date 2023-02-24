#include <DxLib.h>
#include "TitleScene.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "SceneCtl.h"
#include "../input/Mouse.h"
#include "../_debug/_DebugConOut.h"

//--------------------------------------------------------------------------
//���V�[���̍X�V
//	keyData�F���͂����ŐV�̃L�[���
//	keyDataOld�FkeyData�̂P�O�̃L�[���
//--------------------------------------------------------------------------
SCN_ID TitleScene::Update(char* keyData, char* keyDataOld)
{
	//SetBackgroundColor(0, 0, 0);
	lpMouse.Update();
	const auto inputData = lpMouse.GetData(InputType::NOW);
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);
	pos_.x = inputData.at("X���W");
	pos_.y = inputData.at("Y���W");

	sctFlag1_ = false;
	sctFlag2_ = false;
	sctFlag3_ = false;

	//TRACE("x:%d  y:%d\n", pos_.x, pos_.y);//�}�E�X���W�f�o�b�O�\��

	if (inputData.count("X���W") && inputData.count("Y���W"))
	{
		//���ڂP�FGameStart
		auto sctPosA = (lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2;
		auto sctPosB = (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2;
		if ((pos_.x >= sctPosA) && (pos_.y >= sctPosB) &&
			(pos_.x <= sctPosA + lpSize.titleSect1_.x) && (pos_.y <= sctPosB + lpSize.titleSect1_.y))
		{
			sctFlag1_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleScene����SelectScene�Ɉڍs\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//���ʐݒ�
				PlaySoundMem(atfSE_, DX_PLAYTYPE_NORMAL, true);
				//lpGameScene.DataLoad();// �f�[�^���[�h//������������Ă�
				return SCN_ID::SELECT;
			}
		}
		//���ڂQ�FPlayGuide
		auto sctPosC = (lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2;
		auto sctPosD = (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2;
		if ((pos_.x >= sctPosC) && (pos_.y >= sctPosD) &&
			(pos_.x <= sctPosC + lpSize.titleSect2_.x) && (pos_.y <= sctPosD + lpSize.titleSect2_.y))
		{
			sctFlag2_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleScene����GuideScene�Ɉڍs\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//���ʐݒ�
				PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE2), DX_PLAYTYPE_BACK, true);
				return SCN_ID::GUIDE;
			}
		}
		//���ڂR�FDatabese
		auto sctPosE = (lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 2;
		auto sctPosF = (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 2;
		if ((pos_.x >= sctPosE) && (pos_.y >= sctPosF) &&
			(pos_.x <= sctPosE + lpSize.titleSect3_.x) && (pos_.y <= sctPosF + lpSize.titleSect3_.y))
		{
			sctFlag3_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				TRACE("TitleScene����DataBaseScene�Ɉڍs\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//���ʐݒ�
				PlaySoundMem(atfSE_, DX_PLAYTYPE_BACK, true);
				return SCN_ID::DATABASE;
			}
		}
	}
	return SCN_ID::TITLE;
}

//--------------------------------------------------------------------------
//���O���t�B�b�N�̕`��
//--------------------------------------------------------------------------
void TitleScene::Draw(void)
{
	//TRACE("TitleScene�̕`��\n");
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
//��������
//--------------------------------------------------------------------------
bool TitleScene::Init(void)
{
	//TRACE("TitleScene�̏�����\n");
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
//�����\�[�X�̉��
//--------------------------------------------------------------------------
bool TitleScene::Release(void)
{
	TRACE("TitleScene�̉��\n");
	DeleteGraph(titleLogoBO_);
	DeleteGraph(titleLogoBW_);
	DeleteGraph(titleGuideLogoBO_);
	DeleteGraph(titleGuideLogoBW_);
	DeleteGraph(atfSE_);
	DeleteGraph(okSE_);
	return true;
}
