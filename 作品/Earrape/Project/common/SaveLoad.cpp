#include "SaveLoad.h"
#include <stdio.h>
#include "../scene/GameScene.h"
#include "../FilePass.h"
#include "../scene/SceneCtl.h"


void SaveLoad::DataSave(void)
{
	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		DataList[static_cast<int>(SaveData::wep)][num] = lpGameScene.wepOpen_[num];
		DataList[static_cast<int>(SaveData::wepClick)][num] = lpGameScene.WepCnt_[num];
	}
	for (int num = 0; num < static_cast<int>(PERFORM::MAX); num++)
	{
		DataList[static_cast<int>(SaveData::ptr)][num] = lpGameScene.ptrOpen_[num];
	}
	for (int num = 0; num < static_cast<int>(CHARACTER::MAX); num++)
	{
		DataList[static_cast<int>(SaveData::mimiClick)][num] = lpGameScene.MimiCnt_[num];
	}
	DataList[static_cast<int>(SaveData::score)][0] = lpSceneCtl.AllScore_;
	const char* file = SAVE_FILEDATA;
	FILE* fp;
	fopen_s(&fp, file, "wb");
	if (fp == nullptr)
	{
		return;
	}
	fwrite(&DataList, sizeof(int), 256, fp);
	fclose(fp);
}


void SaveLoad::DataLoad(void)
{
	const char* file = SAVE_FILEDATA;
	FILE* fp;
	fopen_s(&fp, file, "rb");
	if (fp == nullptr)
	{
		printf("%sのオープンに失敗しました。\n", file);
		return;
	}
	else
	{
		fread(&DataList, sizeof(int), 256, fp);
	}
	fclose(fp);

	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		wepOpen[num] = DataList[static_cast<int>(SaveData::wep)][num];
		WepCnt[num] = DataList[static_cast<int>(SaveData::wepClick)][num];
	}
	for (int num = 0; num < static_cast<int>(PERFORM::MAX); num++)
	{
		ptrOpen[num] = DataList[static_cast<int>(SaveData::ptr)][num];
	}
	for (int num = 0; num < static_cast<int>(CHARACTER::MAX); num++)
	{
		MimiCnt[num] = DataList[static_cast<int>(SaveData::mimiClick)][num];
	}
	ScoreCnt = DataList[static_cast<int>(SaveData::score)][0];

	return;
}
