#include <DxLib.h>
#include "PerformScene.h"
#include "GameScene.h"
#include "../input/Mouse.h"
#include "../FileSize.h"
#include "../_debug/_DebugConOut.h"
#include "SceneCtl.h"
#include "../FilePass.h"

PerformScene::PerformScene()
{
}

PerformScene::~PerformScene()
{
}

SCN_ID PerformScene::Update(char* keyData, char* keyDataOld)
{
	SetBackgroundColor(0, 0, 0);
	lpMouse.Update();
	const auto inputData = lpMouse.GetData(InputType::NOW);
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);
	iconSpace = { WINDOW_SPACE + lpSize.perItem_.x ,  14 + lpSize.perItem_.y };	// 右アイコンまでの間隔
	if (inputData.count("X座標") && inputData.count("Y座標"))					// カーソルの座標情報取得
	{
		pos_.x = inputData.at("X座標");
		pos_.y = inputData.at("Y座標");
		if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
		{
			for (int num = 0; num < static_cast<int>(Dataitem::MAX); num++)
			{
				if ((pos_.x >= WINDOW_SPACE) &&
					(pos_.y >= 14 + iconSpace.y * num) &&
					(pos_.x <= WINDOW_SPACE + lpSize.perItem_.x) &&
					(pos_.y <= 14 + iconSpace.y * num + lpSize.perItem_.y))
				{
					TRACE("クリック\n");
					if (!effectflag)
					{
						PlaySoundFile(SH_buttonSE, DX_PLAYTYPE_BACK);
						itemNum_ = num;
					}
				}
			}
		}
	}

	if (lpMouse.GetPresCnt(Mouse::RMB) == 1)
	{
		// 右クリックされた瞬間
		return SCN_ID::DATABASE;
	}
 	return SCN_ID::PERFORM;
}

void PerformScene::Draw(void)
{
	iconStart = { lpSize.perItem_.x + WINDOW_SPACE * 3 , WINDOW_SPACE };
	iconSpace = { WINDOW_SPACE + lpSize.perItem_.x ,  14 + lpSize.perItem_.y };	// 右アイコンまでの間隔
	perSpace = { WINDOW_SPACE + lpSize.perData_.x ,  5 + lpSize.perData_.y };
	DrawBox(0, 0, lpSize.perItem_.x + WINDOW_SPACE * 2, lpSceneCtl.scnArea_.y, GetColor(192, 192, 192), true);
	for (int num = 0; num < static_cast<int>(Dataitem::MAX); num++)
	{
		DrawGraph(WINDOW_SPACE, 14 + iconSpace.y * num, ItemImage_[num], true);
		if (itemNum_ == num && itemNum_ != -1)
		{
			int i = 3;
			DrawBox(WINDOW_SPACE + i,
				14 + iconSpace.y * num + i,
				WINDOW_SPACE + lpSize.perItem_.x + i,
				14 + iconSpace.y * num + lpSize.perItem_.y + i,
				0xffff00, false);
		}
	}
	switch (static_cast<Dataitem>(itemNum_))
	{
	case Dataitem::CLICK:
		for (int num = 0; num <= 24; num++)
		{
			int row = num % 2;
			int column;
			if (row == 0)
			{
				column = (num % 24 - (num / 2));
			}
			else
			{
				column = ((num % 24 - 1) - (num / 2));
			}
			if (lpGameScene.ptrOpen_[num] == 0)
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, nonImage_, true);
			}
			else
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, perImage_[num], true);
			}
		}
		break;
	case Dataitem::EAR:
		for (int num = 0; num <= 20; num++)
		{
			int row = num % 2;
			int column;
			if (row == 0)
			{
				column = (num % 20 - (num / 2));
			}
			else
			{
				column = ((num % 20 - 1) - (num / 2));
			}
			if (lpGameScene.ptrOpen_[24 + num])
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, perImage_[24 + num], true);
			}
			else
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, nonImage_, true);
			}
		}
		break;
	case Dataitem::WEAPON:
		for (int num = 0; num <= 5; num++)
		{
			int row = num % 2;
			int column;
			if (row == 0)
			{
				column = (num % 5 - (num / 2));
			}
			else
			{
				column = ((num % 5 - 1) - (num / 2));
			}
			if (lpGameScene.ptrOpen_[24 + 20 + num])
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, perImage_[24 + 20 + num], true);
			}
			else
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, nonImage_, true);
			}
		}
		break;
	case Dataitem::SCORE:
		for (int num = 0; num <= 19; num++)
		{
			int row = num % 2;
			int column;
			if (row == 0)
			{
				column = (num % 19 - (num / 2));
			}
			else
			{
				column = ((num % 19 - 1) - (num / 2));
			}
			if (lpGameScene.ptrOpen_[24 + 20 + 5 + num])
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, perImage_[24 + 20 + 5 + num], true);
			}
			else
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, nonImage_, true);
			}
		}
		break;
	case Dataitem::PERFORM:
		for (int num = 0; num <= 8; num++)
		{
			int row = num % 2;
			int column;
			if (row == 0)
			{
				column = (num % 8 - (num / 2));
			}
			else
			{
				column = ((num % 8 - 1) - (num / 2));
			}
			if (lpGameScene.ptrOpen_[24 + 20 + 5 + 19 + num])
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, perImage_[24 + 20 + 5 + 19 + num], true);
			}
			else
			{
				DrawGraph(iconStart.x + perSpace.x * row, iconStart.y + perSpace.y * column, nonImage_, true);
			}
		}
		break;
	default:
		break;
	}
}

bool PerformScene::Init(void)
{
	ItemImage_[0] = LoadGraph(GH_DATA_CLICK);
	ItemImage_[1] = LoadGraph(GH_DATA_EAR);
	ItemImage_[2] = LoadGraph(GH_DATA_WEAPON);
	ItemImage_[3] = LoadGraph(GH_DATA_SCORE);
	ItemImage_[4] = LoadGraph(GH_DATA_PERFORM);

	perImage_[0] = LoadGraph(GH_DATA_CLICK_1_1);
	perImage_[1] = LoadGraph(GH_DATA_CLICK_1_2);
	perImage_[2] = LoadGraph(GH_DATA_CLICK_1_3);
	perImage_[3] = LoadGraph(GH_DATA_CLICK_1_4);
	perImage_[4] = LoadGraph(GH_DATA_CLICK_2_1);
	perImage_[5] = LoadGraph(GH_DATA_CLICK_2_2);
	perImage_[6] = LoadGraph(GH_DATA_CLICK_2_3);
	perImage_[7] = LoadGraph(GH_DATA_CLICK_2_4);
	perImage_[8] = LoadGraph(GH_DATA_CLICK_3_1);
	perImage_[9] = LoadGraph(GH_DATA_CLICK_3_2);
	perImage_[10] = LoadGraph(GH_DATA_CLICK_3_3);
	perImage_[11] = LoadGraph(GH_DATA_CLICK_3_4);
	perImage_[12] = LoadGraph(GH_DATA_CLICK_4_1);
	perImage_[13] = LoadGraph(GH_DATA_CLICK_4_2);
	perImage_[14] = LoadGraph(GH_DATA_CLICK_4_3);
	perImage_[15] = LoadGraph(GH_DATA_CLICK_4_4);
	perImage_[16] = LoadGraph(GH_DATA_CLICK_5_1);
	perImage_[17] = LoadGraph(GH_DATA_CLICK_5_2);
	perImage_[18] = LoadGraph(GH_DATA_CLICK_5_3);
	perImage_[19] = LoadGraph(GH_DATA_CLICK_5_4);
	perImage_[20] = LoadGraph(GH_DATA_CLICK_6_1);
	perImage_[21] = LoadGraph(GH_DATA_CLICK_6_2);
	perImage_[22] = LoadGraph(GH_DATA_CLICK_6_3);
	perImage_[23] = LoadGraph(GH_DATA_CLICK_6_4);
	perImage_[24] = LoadGraph(GH_DATA_EAR_1_1);
	perImage_[25] = LoadGraph(GH_DATA_EAR_1_2);
	perImage_[26] = LoadGraph(GH_DATA_EAR_1_3);
	perImage_[27] = LoadGraph(GH_DATA_EAR_1_4);
	perImage_[28] = LoadGraph(GH_DATA_EAR_2_1);
	perImage_[29] = LoadGraph(GH_DATA_EAR_2_2);
	perImage_[30] = LoadGraph(GH_DATA_EAR_2_3);
	perImage_[31] = LoadGraph(GH_DATA_EAR_2_4);
	perImage_[32] = LoadGraph(GH_DATA_EAR_3_1);
	perImage_[33] = LoadGraph(GH_DATA_EAR_3_2);
	perImage_[34] = LoadGraph(GH_DATA_EAR_3_3);
	perImage_[35] = LoadGraph(GH_DATA_EAR_3_4);
	perImage_[36] = LoadGraph(GH_DATA_EAR_4_1);
	perImage_[37] = LoadGraph(GH_DATA_EAR_4_2);
	perImage_[38] = LoadGraph(GH_DATA_EAR_4_3);
	perImage_[39] = LoadGraph(GH_DATA_EAR_4_4);
	perImage_[40] = LoadGraph(GH_DATA_EAR_5_1);
	perImage_[41] = LoadGraph(GH_DATA_EAR_5_2);
	perImage_[42] = LoadGraph(GH_DATA_EAR_5_3);
	perImage_[43] = LoadGraph(GH_DATA_EAR_5_4);
	perImage_[44] = LoadGraph(GH_DATA_WEAPON_1);
	perImage_[45] = LoadGraph(GH_DATA_WEAPON_2);
	perImage_[46] = LoadGraph(GH_DATA_WEAPON_3);
	perImage_[47] = LoadGraph(GH_DATA_WEAPON_4);
	perImage_[48] = LoadGraph(GH_DATA_WEAPON_ALL);
	perImage_[49] = LoadGraph(GH_DATA_SCORE_1_1);
	perImage_[50] = LoadGraph(GH_DATA_SCORE_1_2);
	perImage_[51] = LoadGraph(GH_DATA_SCORE_1_3);
	perImage_[52] = LoadGraph(GH_DATA_SCORE_1_4);
	perImage_[53] = LoadGraph(GH_DATA_SCORE_1_5);
	perImage_[54] = LoadGraph(GH_DATA_SCORE_1_6);
	perImage_[55] = LoadGraph(GH_DATA_SCORE_1_7);
	perImage_[56] = LoadGraph(GH_DATA_SCORE_1_8);
	perImage_[57] = LoadGraph(GH_DATA_SCORE_1_9);
	perImage_[58] = LoadGraph(GH_DATA_SCORE_1_10);
	perImage_[59] = LoadGraph(GH_DATA_SCORE_1_11);
	perImage_[60] = LoadGraph(GH_DATA_SCORE_1_12);
	perImage_[61] = LoadGraph(GH_DATA_SCORE_1_1);
	perImage_[62] = LoadGraph(GH_DATA_SCORE_2_2);
	perImage_[63] = LoadGraph(GH_DATA_SCORE_2_3);
	perImage_[64] = LoadGraph(GH_DATA_SCORE_2_4);
	perImage_[65] = LoadGraph(GH_DATA_SCORE_2_5);
	perImage_[66] = LoadGraph(GH_DATA_SCORE_2_6);
	perImage_[67] = LoadGraph(GH_DATA_SCORE_2_7);
	perImage_[68] = LoadGraph(GH_DATA_PERFORM_1);
	perImage_[69] = LoadGraph(GH_DATA_PERFORM_2);
	perImage_[70] = LoadGraph(GH_DATA_PERFORM_3);
	perImage_[71] = LoadGraph(GH_DATA_PERFORM_4);
	perImage_[72] = LoadGraph(GH_DATA_PERFORM_5);
	perImage_[73] = LoadGraph(GH_DATA_PERFORM_6);
	perImage_[74] = LoadGraph(GH_DATA_PERFORM_7);
	perImage_[75] = LoadGraph(GH_DATA_PERFORM_ALL);

	nonImage_ = LoadGraph(GH_DATA_NON);

	iconoffset = 45;
	itemNum_ = -1;

	return true;
}

bool PerformScene::Release(void)
{
	for (int num = 0; num < static_cast<int>(Dataitem::MAX); num++)
	{
		DeleteGraph(ItemImage_[num]);
	}
	for (int num = 0; num < static_cast<int>(PERFORM::MAX); num++)
	{
		DeleteGraph(perImage_[num]);
	}
	DeleteGraph(nonImage_);
	return true;
}
