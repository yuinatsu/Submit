#pragma once
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GuideScene.h"
#include "PerformScene.h"
#include "WeaponScene.h"
#include "../common/SaveLoad.h"
#include "../common/DataID.h"
#include "../SCN_ID.h"

#define lpSceneCtl SceneCtl::GetInstance()

class SceneCtl
{
public:
	static SceneCtl& GetInstance(void)
	{
		static SceneCtl s_Instance;
		return s_Instance;
	}
	void Run(void);

	const Vector2 scnArea_;	// 画面解像度
	//int totalscore;
	int TotalScore_;		//総合得点
	int AllScore_;			//今までに取得した全スコア
	int CVS; //SEボリューム設定

	// 耳
	int charCnt_;
	int mimiImage_[5];

	//フォント画像集
	int NumberFontImage[10];	//数字のハンドル
private:
	SceneCtl();
	~SceneCtl();

	void Update(void);
	void Draw(void);
	bool Init(void);
	bool SysInit(void);		// システムの初期化処理
	bool Release(void);
	void GetKeyData(void);
	void SetupScene(void);	// 1フレーム後のシーンの初期化
	void ReleaseScene(void);	// 現在のシーンの解放

	TitleScene titleScene_;		// TitleSceneクラス
	SelectScene selectScene_;	// SelectSceneクラス
	GameScene gameScene_;		// GameSceneクラス
	ResultScene resultScene_;	// ResultSceneクラス
	GuideScene guideScene_;		// GuideSceneクラス
	DataBaseScene dataBaseScene_;		// DataBaseSceneクラス
	PerformScene performScene_;			// PerformSceneクラス
	WeaponScene weaponScene_;			// WeaponSceneクラス


	char keyBuf_[256];		// キー入力情報
	char keyBufOld_[256];	// 1フレーム前のキー入力情報
	SCN_ID scnID_;			// 現在のシーンID
	SCN_ID oldScnID_;		// 1フレーム前のシーンID
	SCN_ID nextScnID_;		// 1フレーム後のシーンID
	int himeiSE_;
};
