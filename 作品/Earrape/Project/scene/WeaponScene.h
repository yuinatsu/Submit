#pragma once
#include "DataBaseScene.h"
class WeaponScene :
	public DataBaseScene
{
public:
	WeaponScene();
	~WeaponScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// グラフィックの描画
	bool Init(void) override;	// 初期化
	bool Release(void) override;	// リソースの解放
private:
	int wepOpen_[static_cast<int> (WEAPON::MAX)] = { 1,1 };			// 武器開放状況（初期の開放状況は2個）
	int WepCnt_[static_cast<int> (WEAPON::MAX)];					// 武器ごとのクリックカウント

	int wepNum_;			// 武器のナンバー
	bool wepFlag_;			// 武器選択用フラグ

	int IconImage_[static_cast<int> (WEAPON::MAX)];
	int DataImage_[static_cast<int> (WEAPON::MAX)];

	Vector2 iconStart;
	Vector2 iconSpace;
	int WepMax;

	Vector2 pos_;
	int iconoffset;
	bool effectflag;
	int click_SE_;
};

