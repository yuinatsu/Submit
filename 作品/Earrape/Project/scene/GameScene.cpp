#pragma warning(disable : 4996)	// fopenを簡易的に使用してます
#include <DxLib.h>
#include <time.h>
#include "GameScene.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "SceneCtl.h"
#include "../input/Mouse.h"
#include "../_debug/_DebugConOut.h"
#include "../common/ImageMng.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

SCN_ID GameScene::Update(char* keyData, char* keyDataOld)
{
	SetBackgroundColor(255, 255, 255);			// 背景色更新
	lpMouse.Update();							// マウス情報更新
	const auto inputData = lpMouse.GetData(InputType::NOW);		// マウスカーソルの現在の情報取得
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);	// マウスカーソルの過去の情報取得
	Vector2 iconStart = { WINDOW_SPACE + ICON_SPACEX - iconoffset ,  WINDOW_SPACE + ICON_SPACEY - iconoffset };	// 一番左上の武器アイコン
	Vector2 iconSpace = { ICON_SPACEX + lpSize.wepIcon_.x ,  ICON_SPACEY + lpSize.wepIcon_.y };	// 右アイコンまでの間隔
	if (lpMouse.GetPresCnt(Mouse::RMB) == 1 && StartTime != 0)	//ポーズ（２回目押すまでタイマを止める）
	{
		bool oldtiflag = timeflag_;
		timeflag_ = true;
		// 時間停止
		if (oldtiflag == timeflag_)
		{
			StartTime = GetNowCount() - aftime;			// 経過時間減算
			pageCnt_ = 1;
			pageflag_ = false;
			timeflag_ = false;
		}
	}
	if (timeflag_)
	{
		//追尾アイコンがとまるので更新
		if (inputData.count("X座標") && inputData.count("Y座標"))
		{
			pos_.x = inputData.at("X座標");
			pos_.y = inputData.at("Y座標");
		}
		// 選択フラグ
		sctFlag1_ = false;
		sctFlag2_ = false;
		sctFlag3_ = false;
		// 項目座標
		auto sctPosA = (lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 20;
		auto sctPosB = (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 6;

		if (pageflag_ == false)
		{
			if ((pos_.x >= sctPosA) && (pos_.y >= sctPosB) &&
				(pos_.x <= sctPosA + lpSize.titleSect1_.x) && (pos_.y <= sctPosB + lpSize.titleSect1_.y))
			{
				sctFlag1_ = true;
				if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
				{
					Alltime += aftime;
					StartTime = GetNowCount() - aftime;
					timeflag_ = false;
					PlaySoundMem(click_SE_, DX_PLAYTYPE_BACK, true);

				}
			}
			//項目２：PlayGuide
			auto sctPosC = (lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 20;
			auto sctPosD = (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 4;
			if ((pos_.x >= sctPosC) && (pos_.y >= sctPosD) &&
				(pos_.x <= sctPosC + lpSize.titleSect2_.x) && (pos_.y <= sctPosD + lpSize.titleSect2_.y))
			{
				sctFlag2_ = true;
				if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
				{
					PlaySoundMem(click_SE_, DX_PLAYTYPE_BACK, true);

					pageflag_ = true;
				}
			}
			//項目３：Databese
			auto sctPosE = (lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 20;
			auto sctPosF = (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 3;
			if ((pos_.x >= sctPosE) && (pos_.y >= sctPosF) &&
				(pos_.x <= sctPosE + lpSize.titleSect3_.x) && (pos_.y <= sctPosF + lpSize.titleSect3_.y))
			{
				sctFlag3_ = true;
				if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
				{
					Alltime += aftime;
					PlaySoundMem(click_SE_, DX_PLAYTYPE_BACK, true);

					return SCN_ID::TITLE;
				}
			}
		}
	}
	else
	{
		//1秒たったら、タイトルに戻す（仮）
		aftime = GetNowCount() - StartTime;
		if (aftime > 30000 && StartTime != 0)
		{
			Alltime += aftime;
			StartTime = 0;
			// トータルスコア実績開放条件
			lpSceneCtl.AllScore_ += lpSceneCtl.TotalScore_;
			AchievementScore();
			return SCN_ID::RESULT;
		}
		if (inputData.count("X座標") && inputData.count("Y座標"))					// カーソルの座標情報取得
		{
			pos_.x = inputData.at("X座標");
			pos_.y = inputData.at("Y座標");
			if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
			{
				// 武器ウィンドウ内
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
					//DrawGraph(iconStart.x + iconSpace.x * row, iconStart.y + iconSpace.y * column, WeaponImage_[num], true);
					if ((pos_.x >= iconStart.x + iconSpace.x * row) &&
						(pos_.y >= iconStart.y + iconSpace.y * column) &&
						(pos_.x <= iconStart.x + iconSpace.x * row + lpSize.wepIcon_.x) &&
						(pos_.y <= iconStart.y + iconSpace.y * column + lpSize.wepIcon_.y))
					{
						TRACE("クリック\n");
						if (!effectflag)
						{
							wepNum_ = num;
							PlaySoundMem(click_SE_, DX_PLAYTYPE_BACK, true);
						}
					}
				}

				if (wepNum_ != -1)		// 武器が選択されていたら耳を攻撃可能になる
				{
					wepFlag_ = true;
				}
				if (wepFlag_)
				{
					if ((pos_.x >= 360) &&
						(pos_.y >= 100) &&
						(pos_.x <= 360 + 164) &&
						(pos_.y <= 100 + 290))
					{
						if (StartTime == 0)
						{
							// タイマーカウント開始
							StartTime = GetNowCount();
							Limitflag = true;
						}

						TRACE("攻撃モーション\n");
						himeiCnt++;
						/*TRACE("%d\n", himeiCnt % 10);*/
						if (himeiCnt % himeilimit == 0)
						{
							PlaySoundMem(himei_SE_, DX_PLAYTYPE_BACK, true);
							ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, himei_SE_);//音量設定
						}
						if (effectCnt_ > 99)
						{
							effectflag = true;
							effectCnt_ = 0;
							posefct = { pos_.x - 25, pos_.y - ICON_SPACEX / 2 };
						}
						MimiCnt_[lpSceneCtl.charCnt_ - 1]++;		// 耳ごとのクリックカウント加算
						AchievementClick();

						atkflag_ = true;
						AchievementEar();
					}
				}
			}
		}
	}
	effectCnt_ += 2;
	if (effectCnt_ > 99)
	{
		effectflag = false;
	}

	// 武器開放条件
	if (WepCnt_[static_cast<int>(WEAPON::MENBO)] >= 100 && WepCnt_[static_cast<int>(WEAPON::MIMIKAKI)] >= 100)
	{
		wepOpen_[2] = 1;
		ptrOpen_[44] = 1;
		if (WepCnt_[static_cast<int>(WEAPON::LCG)] >= 100)
		{
			wepOpen_[3] = 1;
			ptrOpen_[45] = 1;
			if (WepCnt_[static_cast<int>(WEAPON::BERA)] >= 100)
			{
				wepOpen_[4] = 1;
				ptrOpen_[46] = 1;
				if (WepCnt_[static_cast<int>(WEAPON::OMORI)] >= 100)
				{
					wepOpen_[5] = 1;
					ptrOpen_[47] = 1;
					ptrOpen_[48] = 1;
				}
			}
		}
	}

	//ノドンの更新
	for (auto& ndn : nodon_)
	{
		if (!ndn.isActive)
		{
			continue;
		}
		ndn.pos += ndn.vel;
		ndn.Accel += 0.5f;
		ndn.vel = (ndn.goal - ndn.pos).Normalized() * (NodonSpeed_ + ndn.Accel);
		if (ndn.pos.y > ndn.goal.y)
		{
			ndn.isActive = false;
		}
	}

	// スコア実績開放条件
	AchievementTotalScore();
	AchievementNum();

	return SCN_ID::GAME;
}

void GameScene::Draw(void)
{
	//TRACE("GameSceneの描画\n"); 
	DrawGraph(0, 0, BackGroundImage_, true);
	DrawPause();

	auto scnArea = lpSceneCtl.scnArea_;
	Vector2 windowEnd = { scnArea.x / 3,scnArea.y - WINDOW_SPACE };		// 武器window端
	Vector2 iconStart = { WINDOW_SPACE + ICON_SPACEX - iconoffset ,  WINDOW_SPACE + ICON_SPACEY - iconoffset };	// 一番左上の武器アイコン
	Vector2 iconSpace = { ICON_SPACEX + lpSize.wepIcon_.x ,  ICON_SPACEY + lpSize.wepIcon_.y };	// 右アイコンまでの間隔
	DrawBox(WINDOW_SPACE, WINDOW_SPACE - iconoffset, windowEnd.x + iconoffset, scnArea.y - WINDOW_SPACE, silver_, true);
	if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
	{
		if (atkflag_)
		{
			dmgCnt_ = 30;
		}
	}
	//耳の描画
	if (dmgCnt_ != 0)
	{
		drawOffset_.x = (rand() % 6) - 3;

		DrawGraph(scnArea.x / 2 + drawOffset_.x, (scnArea.y - lpSize.mimiGraph_.y) / 2, lpSceneCtl.mimiImage_[lpSceneCtl.charCnt_- 1], true);

		dmgCnt_--;
	}
	else
	{
		drawOffset_.x = 0;
		DrawGraph(scnArea.x / 2, (scnArea.y - lpSize.mimiGraph_.y) / 2, lpSceneCtl.mimiImage_[lpSceneCtl.charCnt_ - 1], true);
		atkflag_ = false;
	}
	DrawIcon(iconSpace,iconStart);
	DrawNumber(aftime / 1000.0f);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawGuide();
	
	for (auto& ndn : nodon_)
	{
		if (!ndn.isActive)
		{
			continue;
		}
		float angle = atan2(ndn.vel.y, ndn.vel.x);
		DrawRotaGraph(ndn.pos.x, ndn.pos.y, 1.5f, angle + DX_PI_F / 2, WeaponImage_[5], true);
	}

}

bool GameScene::Init(void)
{
	pageCnt_ = 1;

	guideImage_[0] = LoadGraph(GH_GUIDE_2);
	guideImage_[1] = LoadGraph(GH_GUIDE_3);
	guideImage_[2] = LoadGraph(GH_GUIDE_4);
	Vector2 divsize = Vector2{ 48,48 };
	Vector2 divCnt = Vector2{ 5,2 };
	lpImageMng.GetID("Resource/image/blood1.png", "blood1", divsize, divCnt);
	lpImageMng.GetID("Resource/image/blood2.png", "blood2", divsize, divCnt);
	lpImageMng.GetID("Resource/image/blood3.png", "blood3", divsize, divCnt);
	lpImageMng.GetID("Resource/image/Explosion.png", "Explosion", Vector2{ 96,96 }, Vector2{ 18,1 });
	lpImageMng.GetID("Resource/image/Explosion1.png", "Explosion1", Vector2{ 128,128 }, Vector2{ 4,4 });
	lpImageMng.GetID("Resource/image/Thrusts1.png", "Thrusts1", Vector2{ 64,32 }, Vector2{ 1,5 });
	lpImageMng.GetID("Resource/image/Thrusts2.png", "Thrusts2", Vector2{ 64,32 }, Vector2{ 1,5 });
	lpImageMng.GetID("Resource/image/kiss.png", "kiss", Vector2{ 64,64 }, Vector2{ 5,4 });

	posefct = { 0,0 };

	bool rtnFlag = true;
	white_ = GetColor(255, 255, 255);
	black_ = GetColor(0, 0, 0);
	silver_ = GetColor(192, 192, 192);

	lpSceneCtl.TotalScore_ = 0;

	IconImage_[0] = LoadGraph(GH_ATLAS_COTTON);
	IconImage_[1] = LoadGraph(GH_ATLAS_PICK);
	IconImage_[2] = LoadGraph(GH_ATLAS_WRIST_CUTTER_G);
	IconImage_[3] = LoadGraph(GH_ATLAS_DEEPKISS);
	IconImage_[4] = LoadGraph(GH_ATLAS_OMORI);
	IconImage_[5] = LoadGraph(GH_ATLAS_NODON);

	WeaponImage_[0] = LoadGraph(GH_ICON_COTTON);
	WeaponImage_[1] = LoadGraph(GH_ICON_PICK);
	WeaponImage_[2] = LoadGraph(GH_ICON_WRIST_CUTTER_G);
	WeaponImage_[3] = LoadGraph(GH_ICON_DEEPKISS);
	WeaponImage_[4] = LoadGraph(GH_ICON_OMORI);
	WeaponImage_[5] = LoadGraph(GH_ICON_NODON);

	BackGroundImage_ = LoadGraph(GH_BG_GAME);

	iconoffset = 45;
	//フォント画像読み込み
	ScoreImage_ = LoadGraph(GH_LOGO_SCORE_BW);
	SecImage_ = LoadGraph(GH_LOGO_SEC_BW);
	TimeImage_ = LoadGraph(GH_LOGO_TIME_BW);

	AbortBO_ = LoadGraph(GH_LOGO_ABORT_BO);
	AbortBW_ = LoadGraph(GH_LOGO_ABORT_BW);
	section1_BO_ = LoadGraph(GH_LOGO_TITLE_SECTION_1_BO);
	section1_BW_ = LoadGraph(GH_LOGO_TITLE_SECTION_1_BW);
	section2_BO_ = LoadGraph(GH_LOGO_TITLE_SECTION_2_BO);
	section2_BW_ = LoadGraph(GH_LOGO_TITLE_SECTION_2_BW);

	atfSE_ = LoadSoundMem(SH_ATF);
	atkSE_ = LoadSoundMem(SH_ATK);
	explosionSE_ = LoadSoundMem(SH_AexplSE);
	omoriSE_ = LoadSoundMem(SH_OMORI);

	destuctionSE_ = LoadSoundMem(SH_AdestlSE);
	click_SE_ = LoadSoundMem(click_SE);
	BERA_SE_ = LoadSoundMem(SH_ABERASE);
	brooming_SE_ = LoadSoundMem(SH_AbroomingSE);
	himei_SE_ = LoadSoundMem(SH_HIMEI);


	mimiSpd_ = 3;

	dmgCnt_ = 0;
	wepNum_ = -1;
	wepFlag_ = false;

	timeflag_ = false;
	StartTime = 0;
	Limitflag = false;
	EndTime = 10000;	//10000で１０秒

	Alltime = 0;
	StartTime = 0;

	himeiCnt = 0;
	himeilimit = 10;//悲鳴までの回数

	// セーブデータ先の情報格納
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
	ptrCnt = 0;
	for (int num = 0; num < static_cast<int>(PERFORM::MAX); num++)
	{
		ptrOpen_[num] = lpSaveLoad.ptrOpen[num];
		if (ptrOpen_[num])
		{
			ptrCnt++;
		}
	}
	for (int num = 0; num < static_cast<int>(CHARACTER::MAX); num++)
	{
		MimiCnt_[num] = lpSaveLoad.MimiCnt[num];
	}

	srand((unsigned int)time(NULL));

	return rtnFlag;
}

bool GameScene::Release(void)
{
	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		DeleteGraph(WeaponImage_[num]);
	}
	for (int num = 0; num < static_cast<int>(WEAPON::MAX); num++)
	{
		DeleteGraph(IconImage_[num]);
	}

	DeleteGraph(ScoreImage_);
	DeleteGraph(SecImage_);
	DeleteGraph(TimeImage_);

	DeleteGraph(atfSE_);
	DeleteGraph(atkSE_);
	return true;
}


void GameScene::AchievementScore(void)
{
	if (lpSceneCtl.AllScore_ >= 10000000)
	{
		ptrOpen_[61] = 1;
		if (lpSceneCtl.AllScore_ >= 100000000)
		{
			ptrOpen_[62] = 1;
			if (lpSceneCtl.AllScore_ >= 1000000000)
			{
				ptrOpen_[63] = 1;
				if (lpSceneCtl.AllScore_ >= 10000000000)
				{
					ptrOpen_[64] = 1;
					if (lpSceneCtl.AllScore_ >= 100000000000)
					{
						ptrOpen_[65] = 1;
						if (lpSceneCtl.AllScore_ >= 1000000000000)
						{
							ptrOpen_[66] = 1;
							if (lpSceneCtl.AllScore_ >= 10000000000000)
							{
								ptrOpen_[67] = 1;
							}
						}
					}
				}
			}
		}
	}
}

void GameScene::AchievementClick(void)
{
	switch (static_cast<CHARACTER>(lpSceneCtl.charCnt_ - 1))
	{
	case CHARACTER::ISE:
		if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100)
		{
			ptrOpen_[24] = 1;
			if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 1000)
			{
				ptrOpen_[25] = 1;
				if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 10000)
				{
					ptrOpen_[26] = 1;
					if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100000)
					{
						ptrOpen_[27] = 1;
					}
				}
			}
		}
		break;
	case CHARACTER::IMIYA:
		if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100)
		{
			ptrOpen_[28] = 1;
			if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 1000)
			{
				ptrOpen_[29] = 1;
				if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 10000)
				{
					ptrOpen_[30] = 1;
					if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100000)
					{
						ptrOpen_[31] = 1;
					}
				}
			}
		}
		break;
	case CHARACTER::URAYAMA:
		if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100)
		{
			ptrOpen_[32] = 1;
			if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 1000)
			{
				ptrOpen_[33] = 1;
				if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 10000)
				{
					ptrOpen_[34] = 1;
					if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100000)
					{
						ptrOpen_[35] = 1;
					}
				}
			}
		}
		break;
	case CHARACTER::KAKITA:
		if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100)
		{
			ptrOpen_[36] = 1;
			if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 1000)
			{
				ptrOpen_[37] = 1;
				if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 10000)
				{
					ptrOpen_[38] = 1;
					if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100000)
					{
						ptrOpen_[39] = 1;
					}
				}
			}
		}
		break;
	case CHARACTER::KAZIWARA:
		if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100)
		{
			ptrOpen_[40] = 1;
			if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 1000)
			{
				ptrOpen_[41] = 1;
				if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 10000)
				{
					ptrOpen_[42] = 1;
					if (MimiCnt_[lpSceneCtl.charCnt_ - 1] >= 100000)
					{
						ptrOpen_[43] = 1;
					}
				}
			}
		}
		break;
	default:
		break;
	}

}

void GameScene::AchievementEar(void)
{
	switch (static_cast<WEAPON>(wepNum_))
	{
	case WEAPON::MENBO:
		PlaySoundMem(brooming_SE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, brooming_SE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[0] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[1] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[2] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[3] = 1;
					}
				}
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;
		break;

	case WEAPON::MIMIKAKI:
		PlaySoundMem(brooming_SE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, brooming_SE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[4] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[5] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[6] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[7] = 1;
					}
				}
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;
		break;

	case WEAPON::LCG:
		PlaySoundMem(atkSE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, atkSE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[8] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[9] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[10] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[11] = 1;
					}
				}
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;

		break;

	case WEAPON::BERA:
		PlaySoundMem(BERA_SE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, BERA_SE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[12] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[13] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[14] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[15] = 1;
					}
				}
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;
		break;

	case WEAPON::OMORI:

		PlaySoundMem(omoriSE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 10, omoriSE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[16] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[17] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[18] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[19] = 1;
					}
				}
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;

		posefct = { pos_.x - 48,pos_.y - 48 };
		break;

	case WEAPON::NODON:
		PlaySoundMem(destuctionSE_, DX_PLAYTYPE_BACK, true);
		ChangeVolumeSoundMem(255 * lpSceneCtl.CVS / 20, destuctionSE_);//音量設定
		WepCnt_[wepNum_]++;
		if (WepCnt_[wepNum_] >= 100)
		{
			ptrOpen_[20] = 1;
			if (WepCnt_[wepNum_] >= 1000)
			{
				ptrOpen_[21] = 1;
				if (WepCnt_[wepNum_] >= 10000)
				{
					ptrOpen_[22] = 1;
					if (WepCnt_[wepNum_] >= 100000)
					{
						ptrOpen_[23] = 1;
					}
				}
			}
		}
		for (auto& ndn : nodon_)
		{
			if (!ndn.isActive)
			{
				ndn.goal = { static_cast<float>(posefct.x),static_cast<float>(posefct.y) };
				ndn.pos = { static_cast<float>(rnd() % lpSceneCtl.scnArea_.x),-100.0f };
				ndn.vel = (ndn.goal - ndn.pos).Normalized() * NodonSpeed_;
				ndn.Accel = 0.0f;
				ndn.isActive = true;
				break;
			}
		}
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;
		posefct = { pos_.x - 64,pos_.y - 64 };
		break;

	case WEAPON::WEPICON:
		WepCnt_[wepNum_]++;
		lpSceneCtl.TotalScore_ += (wepNum_ + 1) * AtkPoint;
		break;
	default:
		break;
	}
}

void GameScene::AchievementTotalScore(void)
{
	if (lpSceneCtl.TotalScore_ >= 100000)
	{
		ptrOpen_[49] = 1;
		if (lpSceneCtl.TotalScore_ >= 200000)
		{
			ptrOpen_[50] = 1;
			if (lpSceneCtl.TotalScore_ >= 300000)
			{
				ptrOpen_[51] = 1;
				if (lpSceneCtl.TotalScore_ >= 400000)
				{
					ptrOpen_[52] = 1;
					if (lpSceneCtl.TotalScore_ >= 500000)
					{
						ptrOpen_[53] = 1;
						if (lpSceneCtl.TotalScore_ >= 600000)
						{
							ptrOpen_[54] = 1;
							if (lpSceneCtl.TotalScore_ >= 700000)
							{
								ptrOpen_[55] = 1;
								if (lpSceneCtl.TotalScore_ >= 800000)
								{
									ptrOpen_[56] = 1;
									if (lpSceneCtl.TotalScore_ >= 900000)
									{
										ptrOpen_[57] = 1;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (lpSceneCtl.TotalScore_ >= 1000000)
	{
		ptrOpen_[58] = 1;
		if (lpSceneCtl.TotalScore_ >= 5000000)
		{
			ptrOpen_[59] = 1;
			if (lpSceneCtl.TotalScore_ >= 10000000)
			{
				ptrOpen_[60] = 1;
			}
		}
	}
}

void GameScene::AchievementNum(void)
{
	if (ptrCnt >= 10)
	{
		ptrOpen_[68] = 1;
		if (ptrCnt >= 20)
		{
			ptrOpen_[69] = 1;
			if (ptrCnt >= 30)
			{
				ptrOpen_[70] = 1;
				if (ptrCnt >= 40)
				{
					ptrOpen_[71] = 1;
					if (ptrCnt >= 50)
					{
						ptrOpen_[72] = 1;
						if (ptrCnt >= 60)
						{
							ptrOpen_[73] = 1;
							if (ptrCnt >= 70)
							{
								ptrOpen_[74] = 1;
								if (ptrCnt >= static_cast<int>(PERFORM::MAX))
								{
									ptrOpen_[75] = 1;
								}
							}
						}
					}

				}
			}
		}
	}
}

void GameScene::DrawNumber(float remainTime)
{
	//スコアの表示
	DrawGraph(lpSceneCtl.scnArea_.x - TotalScoreSizeX, 0, ScoreImage_, true);
	//TotalScore_を描画・取り合えず６桁とする
	std::string STotalscore_ = std::to_string(lpSceneCtl.TotalScore_);
	//STotalscore_が６桁未満の場合、０を挿入し６桁にする。
	while (STotalscore_.length() != ScoreDigit)
	{
		STotalscore_.insert(0, "0");
	}
	for (int i = 0; i < ScoreDigit; i++)
	{
		int L = atoi(STotalscore_.substr(i, 1).c_str());	//残り時間の内、一桁を取り出す
		DrawGraph(lpSceneCtl.scnArea_.x - TotalScoreSizeX + FontNumSizeX * i, ScoreSizeY, lpSceneCtl.NumberFontImage[L], true);
	}


	//secの表示
	DrawGraph(lpSceneCtl.scnArea_.x - SecSizeX, lpSceneCtl.scnArea_.y - SecSizeY, SecImage_, true);
	//TIMEの表示
	DrawGraph(lpSceneCtl.scnArea_.x - (FontNumSizeX * 2 + SecSizeX + TimeSizeX), lpSceneCtl.scnArea_.y - FontNumSizeY - TimeSizeY, TimeImage_, true);

	//残り時間の表示
	if (Limitflag)
	{
		std::string SLimitTime = std::to_string(LimitTime - remainTime);	//floatをstring型に変換
		//耳をクリックすると時間が減り始める
		//SLimitTime.erase(2, 1);		//　「．」（ピリオド）を削除
		if (LimitTime - remainTime > 10.0f)
		{
			for (int i = 0; i < LimitTimeDigit; i++)
			{
				//残り時間の内、少数第二位まで描画
				int L = atoi(SLimitTime.substr(i, 1).c_str());	//残り時間の内、一桁を取り出す
				DrawGraph(lpSceneCtl.scnArea_.x - TotalLimitTimeSizeX + FontNumSizeX * i - SecSizeX, lpSceneCtl.scnArea_.y - FontNumSizeY, lpSceneCtl.NumberFontImage[L], true);
			}
		}
		else
		{
			int L = atoi(SLimitTime.substr(0, 1).c_str());	//残り時間の内、一桁を取り出す
			DrawGraph(lpSceneCtl.scnArea_.x - TotalLimitTimeSizeX + FontNumSizeX  - SecSizeX, lpSceneCtl.scnArea_.y - FontNumSizeY, lpSceneCtl.NumberFontImage[L], true);
		}
	}
	else
	{
		//耳をクリックする前は３０秒固定
		for (int num = 1; num <= LimitTimeDigit-1; num++)
		{
			DrawGraph(lpSceneCtl.scnArea_.x - FontNumSizeX * num - SecSizeX, lpSceneCtl.scnArea_.y - FontNumSizeY, lpSceneCtl.NumberFontImage[0], true);
		}
		//３をひとつ描画
		DrawGraph(lpSceneCtl.scnArea_.x - FontNumSizeX * LimitTimeDigit- SecSizeX, lpSceneCtl.scnArea_.y - FontNumSizeY, lpSceneCtl.NumberFontImage[3], true);
	}
}

void GameScene::DrawEffect(int num)
{
	int Divide = 10;
	int nine = 9;
	if (effectflag)
	{
		//効果音を追加する場合、ここで追加すると,音が遅れるのでダメージカウントのところに書いて
		if (num == 0)
		{
			/*TRACE("%d\n", effectCnt_ / 20 % 5);*/

			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("blood2")[effectCnt_ / Divide % nine], true);

			if (effectCnt_ / Divide % nine > 7)//落ちるエフェクト
			{
				DrawGraph(posefct.x, ++posefct.y, lpImageMng.GetID("blood2")[nine], true);
			}
			lpSceneCtl.TotalScore_++;//持続ダメージ処理
		}
		if (num == 1)
		{
			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("blood2")[effectCnt_ / Divide % nine], true);
			if (effectCnt_ / Divide % nine > 7)	//落ちるエフェクト
			{
				DrawGraph(posefct.x, posefct.y += 4, lpImageMng.GetID("blood2")[nine], true);
			}
			lpSceneCtl.TotalScore_++;//持続ダメージ処理
		}
		if (num == 2)
		{
			DrawGraph(posefct.x, posefct.y + Divide, lpImageMng.GetID("Thrusts1")[effectCnt_ / (Divide + Divide) % (Divide / 2)], true);
			DrawGraph(posefct.x, posefct.y + Divide, lpImageMng.GetID("Thrusts2")[effectCnt_ / (Divide + Divide) % (Divide / 2)], true); //Drawcallがおおかったら、このDRAWは消してもいい
			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("blood1")[effectCnt_ / Divide % Divide], true);

			lpSceneCtl.TotalScore_ += 5;
		}
		if (num == 3)
		{
			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("kiss")[effectCnt_ / 6 % (nine * 2)], true);
		}
		if (num == 4)
		{
			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("Explosion1")[effectCnt_ / 12 % 8], true);

		}
		if (num == 5)
		{
			DrawGraph(posefct.x, posefct.y, lpImageMng.GetID("Explosion")[effectCnt_ / 6 % (nine * 2)], true);
			if (effectCnt_ > 65)
			{
				lpSceneCtl.TotalScore_ += 500;//持続ダメージ処理
			}
		}
	}
}

void GameScene::DrawGuide(void)
{
	//ポーズ時に背景と被らないようにここにDrawしている
	if (pageflag_)
	{
		DrawRotaGraph2(30, 30, 0, 0, cntdow, 0, guideImage_[(pageCnt_ - 1)], true);
		if (cntdow < 0.9f)
		{
			cntdow += 0.105f;
		}
		if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
		{
			if ((pos_.x >= 360) &&
				(pos_.y >= 0) &&
				(pos_.x <= 720) &&
				(pos_.y <= 480))
			{
				if (pageCnt_ > 0)
				{
					if (pageCnt_ < 3)
					{
						pageCnt_++;
						PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE1), DX_PLAYTYPE_BACK, true);

					}
					else
					{
						pageCnt_ = 1;
						PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE3), DX_PLAYTYPE_BACK, true);

						pageflag_ = false;
					}
				}
			}
			if ((pos_.x >= 0) &&
				(pos_.y >= 0) &&
				(pos_.x <= 360) &&
				(pos_.y <= 480))
			{
				if (pageCnt_ > 1)
				{
					pageCnt_--;
					PlaySoundMem(LoadSoundMem(GUIDE_BOOK_SE1), DX_PLAYTYPE_BACK, true);

				}
			}
		}

	}
	else
	{
		if (cntdow > 0.1f)
		{
			cntdow -= 0.1f;
			DrawRotaGraph2(30, 30, 0, 0, cntdow, 0, guideImage_[(pageCnt_ - 1)], true);
		}
		else
		{
			cntdow = 0;
		}
	}
}

void GameScene::DrawPause(void)
{
	if (timeflag_)
	{
		if (!sctFlag1_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 6, section1_BW_, true);
		}
		if (!sctFlag2_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 4, section2_BW_, true);
		}
		if (!sctFlag3_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 3, AbortBW_, true);
		}

		if (sctFlag1_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect1_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect1_.y * 1)) / 6, section1_BO_, true);
		}
		if (sctFlag2_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect2_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect2_.y * 5)) / 4, section2_BO_, true);
		}
		if (sctFlag3_)
		{
			DrawGraph((lpSceneCtl.scnArea_.x - lpSize.titleSect3_.x) / 20, (lpSceneCtl.scnArea_.y + (lpSize.titleSect3_.y * 9)) / 3, AbortBO_, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 125);

	}
}

void GameScene::DrawIcon(Vector2 iconSpace, Vector2 iconStart)
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
		DrawGraph(iconStart.x + iconSpace.x * row, iconStart.y + iconSpace.y * column, IconImage_[num], true);

		if (wepNum_ == num)
		{
			int i = 3;
			DrawBox(iconStart.x + iconSpace.x * row + i,
				iconStart.y + iconSpace.y * column + i,
				iconStart.x + iconSpace.x * row + lpSize.wepIcon_.x + i,
				iconStart.y + iconSpace.y * column + lpSize.wepIcon_.y + i,
				0xffff00, false);
			DrawGraph(pos_.x, pos_.y - ICON_SPACEY * 2, WeaponImage_[num], true);
			DrawEffect(num);
		}
	}
}
