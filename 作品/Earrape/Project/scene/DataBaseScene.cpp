#include <DxLib.h>
#include "DataBaseScene.h"
#include "../FilePass.h"
#include "../input/Mouse.h"
#include "SceneCtl.h"
#include "../FileSize.h"
#include "../_debug/_DebugConOut.h"

DataBaseScene::DataBaseScene()
{
}

DataBaseScene::~DataBaseScene()
{
}

SCN_ID DataBaseScene::Update(char* keyData, char* keyDataOld)
{
	lpMouse.Update();			// �}�E�X�X�V
	const auto inputData = lpMouse.GetData(InputType::NOW);		// �}�E�X�̌��݂̓��͏��擾
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);	// �}�E�X�̉ߋ��̓��͏��擾
	// �}�E�X�J�[�\�����W�X�V
	pos_.x = inputData.at("X���W");
	pos_.y = inputData.at("Y���W");

	// �I���t���O
	sctFlag1_ = false;
	sctFlag2_ = false;

	if (inputData.count("X���W") && inputData.count("Y���W"))
	{
		//���ڂP�FWEAPON
		// ���ڍ��W
		auto sctPosA = (lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2;
		auto sctPosB = (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2;
		// ���ړ��ɃJ�[�\�������邩
		if ((pos_.x >= sctPosA) && (pos_.y >= sctPosB) &&
			(pos_.x <= sctPosA + lpSize.titleSect1_.x) && (pos_.y <= sctPosB + lpSize.titleSect1_.y))
		{
			sctFlag1_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				// ���N���b�N���ꂽ��V�[���ڍs
				TRACE("DataBaseScene����WeaponScene�Ɉڍs\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//���ʐݒ�
				PlaySoundMem(atfSE_, DX_PLAYTYPE_NORMAL, true);
				return SCN_ID::WEAPON;
			}
		}
		//���ڂQ�FPERFORM
		// ���ڍ��W
		auto sctPosC = (lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2;
		auto sctPosD = (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2;
		// ���ړ��ɃJ�[�\�������邩
		if ((pos_.x >= sctPosC) && (pos_.y >= sctPosD) &&
			(pos_.x <= sctPosC + lpSize.titleSect2_.x) && (pos_.y <= sctPosD + lpSize.titleSect2_.y))
		{
			sctFlag2_ = true;
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				// ���N���b�N���ꂽ��V�[���ڍs
				TRACE("DataBaseScene����PerformScene�Ɉڍs\n");
				ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 100, atfSE_);//���ʐݒ�
				PlaySoundMem(atfSE_, DX_PLAYTYPE_NORMAL, true);
				return SCN_ID::PERFORM;
			}
		}
	}
	if (lpMouse.GetPresCnt(Mouse::RMB) == 1)
	{
		// �E�N���b�N���ꂽ�u��
		PlaySoundFile(SH_buttonSE, DX_PLAYTYPE_BACK);
		return SCN_ID::TITLE;
	}
	blinkCnt_++;
	return SCN_ID::DATABASE;
}

void DataBaseScene::Draw(void)
{
	DrawGraph(0, 0, dbbg_, true);		// �w�i

	DrawGraph(0, 0, dbLogoBW_, true);	// ���S
	if ((blinkCnt_ / 20) % 2)
	{
		DrawGraph(0, 0, dbLogoBO_, true);	// �_�ŗp
	}

	int section1 = 0;
	int section2 = 0;

	section1 = sctFlag1_ ? section1_BO_ : section1_BW_;
	section2 = sctFlag2_ ? section2_BO_ : section2_BW_;

	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2, section1, true);
	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2, section2, true);

	//if (!sctFlag1_)
	//{
	//	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2, section1_BW_, true);
	//}
	//if (!sctFlag2_)
	//{
	//	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2, section2_BW_, true);
	//}
	//if (sctFlag1_)
	//{
	//	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 2, section1_BO_, true);
	//}
	//if (sctFlag2_)
	//{
	//	DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 2, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 2, section2_BO_, true);
	//}
}

bool DataBaseScene::Init(void)
{
	blinkCnt_ = 0;		// �_�ŗp

	// �`���
	dbbg_ = LoadGraph(GH_BG_DATABESE);
	dbLogoBO_ = LoadGraph(GH_LOGO_DATABASE_BO);
	dbLogoBW_ = LoadGraph(GH_LOGO_DATABASE_BW);
	section1_BO_ = LoadGraph(GH_LOGO_TITLE_DATABASE_1_BO);
	section1_BW_ = LoadGraph(GH_LOGO_TITLE_DATABASE_1_BW);
	section2_BO_ = LoadGraph(GH_LOGO_TITLE_DATABASE_2_BO);
	section2_BW_ = LoadGraph(GH_LOGO_TITLE_DATABASE_2_BW);
	// ���y��
	atfSE_ = LoadSoundMem(SH_ATF);
	PlaySoundFile(SH_buttonSE, DX_PLAYTYPE_BACK);
	return true;
}

bool DataBaseScene::Release(void)
{
	// �`���
	DeleteGraph(dbbg_);
	DeleteGraph(dbLogoBO_);
	DeleteGraph(dbLogoBW_);
	// ���y��
	DeleteGraph(atfSE_);
	return true;
}
