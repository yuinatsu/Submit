#pragma once
#include "../common/DataID.h"

#define lpSaveLoad SaveLoad::GetInstance()

// セーブ用データ
enum class SaveData
{
	wep,			// 武器開放状況
	ptr,			// 実績開放状況
	wepClick,		// 武器クリック回数
	mimiClick,		// 耳クリック回数
	score,			// トータルスコア
	MAX
};

class SaveLoad
{
public:
	static SaveLoad& GetInstance(void)
	{
		static SaveLoad s_Instance;
		return s_Instance;
	}
	SaveLoad() {};
	~SaveLoad() {};
	void DataSave();			// データセーブ
	void DataLoad();			// データロード

	int DataList[static_cast<int>(SaveData::MAX)][static_cast<int>(PERFORM::MAX)];			// データ配列

	int wepOpen[static_cast<int> (WEAPON::MAX)] = {1,1};		// 武器開放状況
	int ptrOpen[static_cast<int> (PERFORM::MAX)] = {};			// 実績開放状況
	int WepCnt[static_cast<int> (WEAPON::MAX)] = {};			// 武器ごとのクリックカウント
	int MimiCnt[static_cast<int> (CHARACTER::MAX)] = {};		// 耳ごとのクリックカウント
	int ScoreCnt;												// トータルのスコアカウント
};

//SaveLoadで謎の例外スローが出た場合
// SceneCtl::Drawの内部にある_dbgStartDraw()で例外スローが発生する場合があります。
// セーブするための変数の型を、SaveLoadの内部にあるfread(&DataList, sizeof(int), 256, fp)の"sizeof(int)"の型に合わせれば対処可能です。
