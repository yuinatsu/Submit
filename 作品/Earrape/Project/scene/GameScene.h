#pragma once
#include <memory>
#include<random>
#include "BaseScene.h"
#include "../SCN_ID.h"
#include "../input/Input.h"
#include "WeaponScene.h"
#include "../common/Vector2.h"
#include"../common/Geometry.h"

#define WINDOW_SPACE (20)	// WINウィンドウと武器ウィンドウの間隔
#define ICON_SPACEX (48)	// アイコン同士のX軸間隔		//追尾アイコンをマウス合わせる
#define ICON_SPACEY (32)	// アイコン同士のY軸間隔

#define lpGameScene GameScene::GetInstance()
constexpr float LimitTime = 30.0f;
//画像の大きさ定義
constexpr int ScoreSizeX = 190;
constexpr int ScoreSizeY = 30;
constexpr int SecSizeX = 80;
constexpr int SecSizeY = 25;
constexpr int TimeSizeX = 150;
constexpr int TimeSizeY = 30;

//ノドンの定義
constexpr float NodonSpeed_ = 5.0f;

class SaveLoad;
class WeaponScene;
class PerformScene;

struct Nodon
{
	Position2 pos;
	Position2 goal;
	Position2 vel;
	float Accel = 0.0f;
	bool isActive = false;
};

class GameScene :
    public BaseScene
{
public:
	static GameScene& GetInstance(void)
	{
		static GameScene s_Instance;
		return s_Instance;
	}
	GameScene();
	~GameScene();

	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;
	bool Init(void) override;
	bool Release(void) override;
	Vector2 scrSize_;	// 画面サイズ
private:
	bool pageflag_;
	int pageCnt_;			// ページカウンター
	int guideImage_[3];

	void AchievementScore(void);
	void AchievementClick(void);
	void AchievementEar(void);
	void AchievementTotalScore(void);
	void AchievementNum(void);


	void DrawNumber(float remainTime);	//数字の描画
	void DrawEffect(int num);			//エフェクトの描画
	void DrawGuide(void);				//ガイドの描画
	void DrawPause(void);				//ポーズの描画
	void DrawIcon(Vector2 iconSpace,Vector2 iconStart);				//アイコンの描画

	int WeaponImage_[static_cast<int> (WEAPON::MAX)];	//武器の画像を入れるところ。enumのＭＡＸが武器の数
	int IconImage_[static_cast<int> (WEAPON::MAX)];
	int effectImage_[static_cast<int> (WEAPON::MAX)];
	int iconoffset;
	int BackGroundImage_;	//背景 

	int mimiSpd_;

	int atfSE_;		// A.TフィールドのSE
	int atkSE_;
	int explosionSE_;
	int destuctionSE_;		//仮
	int click_SE_;
	int BERA_SE_;
	int brooming_SE_;
	int omoriSE_;
	int himei_SE_;

	int white_;
	int black_;
	int silver_;
	int dmgCnt_;			// 攻撃したときのリアクション用カウンター ※仮実装
	int wepNum_;			// 武器のナンバー
	bool wepFlag_;			// 武器選択用フラグ
	bool atkflag_;			// 攻撃フラグ
	Vector2 drawOffset_;	// 演出用　※仮実装
	Vector2 pos_;			//マウスの座標　ゲーム用？

	// セーブ先の情報
	int wepOpen_[static_cast<int> (WEAPON::MAX)] = { 1,1,1,1,1,1 };			// 武器開放状況（初期の開放状況は2個）
	int ptrOpen_[static_cast<int> (PERFORM::MAX)] = 
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1};				// 実績開放状況
	int WepCnt_[static_cast<int> (WEAPON::MAX)];					// 武器ごとのクリックカウント
	int MimiCnt_[static_cast<int> (CHARACTER::MAX)];				// 耳ごとのクリックカウント
	int ptrCnt;														// 実績開放数
	int WepMax;														// 使用できる武器の数

	Nodon nodon_[16];			//ノドン
	std::random_device rnd;		//乱数

	//時間関係
	int StartTime;
	int EndTime;	//終了時間
	int aftime;		//経過時間
	int Alltime;	//ゲームの合計プレイ時間
	bool timeflag_;		//時間停止フラグ
	bool Limitflag;		//制限時間が減り始めるフラグ
	//int pusReturnTime;	//停止時間の保持

	float cntdow;

	//フォント画像集
	int ScoreImage_;			//SCOREのハンドル
	int SecImage_;				//secのハンドル
	int TimeImage_;				//TIMEのハンドル
	
	int himeiCnt;
	int himeilimit;

	bool effectflag;
	int effectCnt_;	//エフェクトの時間
	Vector2 posefct;

	int  AbortBO_, AbortBW_, section1_BO_, section1_BW_, section2_BO_, section2_BW_;
	bool sctFlag1_, sctFlag2_, sctFlag3_;

	friend SaveLoad;
	friend WeaponScene;
	friend PerformScene;
};

