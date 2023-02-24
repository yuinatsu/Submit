#include <DxLib.h>
#include "WeaponScene.h"
#include "../input/Mouse.h"
#include "GameScene.h"
#include "SceneCtl.h"
#include "../FileSize.h"
#include "../_debug/_DebugConOut.h"
#include "../FilePass.h"

WeaponScene::WeaponScene()
{
}

WeaponScene::~WeaponScene()
{
}

SCN_ID WeaponScene::Update(char* keyData, char* keyDataOld)
{
	SetBackgroundColor(255, 255, 255);
	lpMouse.Update();
	const auto inputData = lpMouse.GetData(InputType::NOW);
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);
	iconStart = { WINDOW_SPACE + ICON_SPACEX - iconoffset ,  WINDOW_SPACE + ICON_SPACEY - iconoffset };	// 一番左上の武器アイコン
	iconSpace = { ICON_SPACEX + lpSize.wepIcon_.x ,  ICON_SPACEY + lpSize.wepIcon_.y };	// 右アイコンまでの間隔
	if (inputData.count("X座標") && inputData.count("Y座標"))					// カーソルの座標情報取得
	{
		pos_.x = inputData.at("X座標");
		pos_.y = inputData.at("Y座標");
		if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
		{
			for (int num = 0; num < WepMax; num++)
			{
				int row = num % 2;
				int column;
				if (row == 0)
				{
					column = (num % WepMax - (num / 2));
				}
				else
				{
					column = ((num % WepMax - 1) - (num / 2));
				}
				if ((pos_.x >= iconStart.x + iconSpace.x * row) &&
					(pos_.y >= iconStart.y + iconSpace.y * column) &&
					(pos_.x <= iconStart.x + iconSpace.x * row + lpSize.wepIcon_.x) &&
					(pos_.y <= iconStart.y + iconSpace.y * column + lpSize.wepIcon_.y))
				{
					TRACE("クリック\n");
					if (!effectflag)
					{
						PlaySoundMem(click_SE_, DX_PLAYTYPE_BACK, true);
						wepNum_ = num;
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
	return SCN_ID::WEAPON;
}

void WeaponScene::Draw(void)
{
	auto scnArea = lpSceneCtl.scnArea_;
	Vector2 windowEnd = { scnArea.x / 3,scnArea.y - WINDOW_SPACE };		// 武器window端
	iconStart = { WINDOW_SPACE + ICON_SPACEX - iconoffset ,  WINDOW_SPACE + ICON_SPACEY - iconoffset };	// 一番左上の武器アイコン
	iconSpace = { ICON_SPACEX + lpSize.wepIcon_.x ,  ICON_SPACEY + lpSize.wepIcon_.y };	// 右アイコンまでの間隔
	DrawBox(WINDOW_SPACE, WINDOW_SPACE - iconoffset, windowEnd.x + iconoffset, scnArea.y - WINDOW_SPACE, GetColor(192, 192, 192), true);
	for (int num = 0; num < WepMax; num++)
	{
		int row = num % 2;
		int column;
		if (row == 0)
		{
			column = (num % WepMax - (num / 2));
		}
		else
		{
			column = ((num % WepMax - 1) - (num / 2));
		}
		DrawGraph(iconStart.x + iconSpace.x * row, iconStart.y + iconSpace.y * column, IconImage_[num], true);
		if (wepNum_ == num && wepNum_ != -1)
		{
			int i = 3;
			DrawBox(iconStart.x + iconSpace.x * row + i,
				iconStart.y + iconSpace.y * column + i,
				iconStart.x + iconSpace.x * row + lpSize.wepIcon_.x + i,
				iconStart.y + iconSpace.y * column + lpSize.wepIcon_.y + i,
				0xffff00, false);
		}
	}
	switch (static_cast<WEAPON>(wepNum_))
	{
	case WEAPON::MENBO:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[0], true);
		break;
	case WEAPON::MIMIKAKI:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[1], true);
		break;
	case WEAPON::LCG:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[2], true);
		break;
	case WEAPON::BERA:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[3], true);
		break;
	case WEAPON::OMORI:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[4], true);
		break;
	case WEAPON::NODON:
		DrawGraph((scnArea.x - iconSpace.x) / 2, (scnArea.y - lpSize.wepData_.y) / 2, DataImage_[5], true);
		break;
	default:
		break;
	}
}

bool WeaponScene::Init(void)
{
	IconImage_[0] = LoadGraph(GH_ATLAS_COTTON);
	IconImage_[1] = LoadGraph(GH_ATLAS_PICK);
	IconImage_[2] = LoadGraph(GH_ATLAS_WRIST_CUTTER_G);
	IconImage_[3] = LoadGraph(GH_ATLAS_DEEPKISS);
	IconImage_[4] = LoadGraph(GH_ATLAS_OMORI);
	IconImage_[5] = LoadGraph(GH_ATLAS_NODON);
	DataImage_[0] = LoadGraph(GH_DATA_COTTON);
	DataImage_[1] = LoadGraph(GH_DATA_PICK);
	DataImage_[2] = LoadGraph(GH_DATA_WRIST_CUTTER_G);
	DataImage_[3] = LoadGraph(GH_DATA_DEEPKISS);
	DataImage_[4] = LoadGraph(GH_DATA_OMORI);
	DataImage_[5] = LoadGraph(GH_DATA_NODON);

	iconoffset = 45;

	wepNum_ = -1;
	WepMax = 0;
	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		wepOpen_[num] = lpSaveLoad.wepOpen[num];
		WepCnt_[num] = lpSaveLoad.WepCnt[num];
		if (wepOpen_[num])
		{
			WepMax++;
		}
	}
	click_SE_ = LoadSoundMem(click_SE);
	return true;
}

bool WeaponScene::Release(void)
{
	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		DeleteGraph(IconImage_[num]);
		DeleteGraph(DataImage_[num]);
	}
	return true;
}
