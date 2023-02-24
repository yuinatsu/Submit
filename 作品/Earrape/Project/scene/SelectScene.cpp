#include <DxLib.h>
#include<cmath>
#include "SelectScene.h"
#include "SceneCtl.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "../input/Mouse.h"
#include "../_debug/_DebugConOut.h"

//--------------------------------------------------------------------------
//●シーンの更新
//	keyData：入力した最新のキー情報
//	keyDataOld：keyDataの１つ前のキー情報
//--------------------------------------------------------------------------
SCN_ID SelectScene::Update(char* keyData, char* keyDataOld)
{
	lpMouse.Update();
	const auto inputData = lpMouse.GetData(InputType::NOW);
	const auto inputDataOld = lpMouse.GetData(InputType::OLD);
	pos_.x = inputData.at("X座標");
	pos_.y = inputData.at("Y座標");

	if (inputData.count("X座標") && inputData.count("Y座標"))
	{
		if (pos_.x >= 330 && 
			pos_.y >= 480 - StartSizeY &&
			pos_.x <= 330 + StartSizeX &&
			pos_.y <= 480)
		{
			if (!Rflag_ && !Lflag_)
			{
				if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
				{
					for (int i = 1; i <= static_cast<int>(CHARACTER::MAX); i++)
					{
						if (i == pageCnt_)
						{
							lpSceneCtl.charCnt_ = i;
							pageCnt_ = 1;
						}
					}
					PlaySoundFile(SH_buttonSE, DX_PLAYTYPE_BACK);
					return SCN_ID::GAME;
				}
			}
		}
	}

	if (pos_.x >= 720 - ArrowSizeX &&
		pos_.y >= 480 - ArrowSizeY &&
		pos_.x <= 720 &&
		pos_.y <= 480 &&
		lpMouse.GetPresCnt(Mouse::LMB) == 1)
	{
		if (!Rflag_ && !Lflag_)
		{
			// 右クリックされた瞬間
			if (pageCnt_ > 1)
			{
				//真ん中の耳
				Position2 Crad = { RightEarPos_X - CenterEarPos_X, LREarPos_Y - CenterEarPos_Y };
				CenterEar.vel = Crad.Normalized() * (EarSpeed + CenterEar.Accel_);
				CenterEar.MoveFlagR = true;
				//右の耳
				Position2 Rrad = { RUear.x - RightEarPos_X  , RUear.y - LREarPos_Y };
				RightEar.vel = Rrad.Normalized() * (EarSpeed + RightEar.Accel_);
				RightEar.MoveFlagR = true;
				//左の耳
				Position2 Lrad = { CenterEarPos_X - LeftEarPos_X  , CenterEarPos_Y - LREarPos_Y };
				LeftEar.vel = Lrad.Normalized() * (EarSpeed + LeftEar.Accel_);
				LeftEar.MoveFlagR = true;
				//追加の耳
				Position2 Arad = { LeftEarPos_X - LUear.x , LREarPos_Y - LUear.y };
				AddEar.vel = Arad.Normalized() * (EarSpeed * 2.0f + AddEar.Accel_);
				AddEar.pos = { -EarSizeX, -EarSizeY };
				AddEar.MoveFlagR = true;

				Rflag_ = true;
				PlaySoundMem(Select_SE_, DX_PLAYTYPE_BACK, true);
			}
		}
	}
	if (pos_.x >= 60 &&
		pos_.y >= 480 - ArrowSizeY &&
		pos_.x <= 60 + ArrowSizeX &&
		pos_.y <= 480&&
		lpMouse.GetPresCnt(Mouse::LMB) == 1)
	{
		// 左クリックされた瞬間
		if (pageCnt_ < static_cast<int>(CHARACTER::MAX))
		{
			if (!Rflag_ && !Lflag_)
			{
				//真ん中の耳
				Position2 Crad = { LeftEarPos_X - CenterEarPos_X, LREarPos_Y - CenterEarPos_Y };
				CenterEar.vel = Crad.Normalized() * (EarSpeed + CenterEar.Accel_);
				CenterEar.MoveFlagL = true;
				//右の耳
				Position2 Rrad = { CenterEarPos_X - RightEarPos_X  , CenterEarPos_Y - LREarPos_Y };
				RightEar.vel = Rrad.Normalized() * (EarSpeed + RightEar.Accel_);
				RightEar.MoveFlagL = true;
				//左の耳
				Position2 Lrad = { LUear.x- LeftEarPos_X  ,  LUear.y- LREarPos_Y  };
				LeftEar.vel = Lrad.Normalized() * (EarSpeed + LeftEar.Accel_);
				LeftEar.MoveFlagL = true;
				//追加の耳
				Position2 Arad = { RightEarPos_X - RUear.x , LREarPos_Y - RUear.y };
				AddEar.vel = Arad.Normalized() * (EarSpeed*2.0f+ AddEar.Accel_);
				AddEar.pos = { 840, -EarSizeY };
				AddEar.MoveFlagL = true;


				Lflag_ = true;
				PlaySoundMem(Select_SE_, DX_PLAYTYPE_BACK, true);
			}
		}
	}
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
	{
		return SCN_ID::TITLE;
	}

	if (ImageChangeF_)
	{
		if (Rflag_)
		{
			pageCnt_--;
			Rflag_ = false;
		}
		else if (Lflag_)
		{
			pageCnt_++;
			Lflag_ = false;
		}
		ImageChangeF_ = false;
	}

	blinkCnt_++;

	EarMoveR();
	EarMoveL();
	return SCN_ID::SELECT;
}

void SelectScene::EarMoveR(void)
{
	//右に移動する
	if (CenterEar.MoveFlagR)
	{
		//真ん中の耳
		if (CenterEar.pos.x <= RightEarPos_X &&
			CenterEar.pos.y >= LREarPos_Y)
		{
			CenterEar.pos += CenterEar.vel;
			CenterEar.Accel_ += EarAccel;
			Position2 Crad = { RightEarPos_X - CenterEarPos_X, LREarPos_Y - CenterEarPos_Y };
			CenterEar.vel = Crad.Normalized() * (EarSpeed + CenterEar.Accel_);
		}
		else
		{
			CenterEar.pos = { RightEarPos_X,LREarPos_Y };
			CenterEar.Accel_ = 0.0f;
			CenterEar.MoveFlagR = false;
		}
	}
	if (RightEar.MoveFlagR)
	{
		//右の耳
		if (RightEar.pos.x <= RUear.x &&
			RightEar.pos.y >= RUear.y)
		{
			RightEar.pos += RightEar.vel;
			RightEar.Accel_ += EarAccel;
			Position2 Rrad = { RUear.x - RightEarPos_X  , RUear.y - LREarPos_Y };
			RightEar.vel = Rrad.Normalized() * (EarSpeed + RightEar.Accel_);
		}
		else
		{
			RightEar.pos = { 0,-500 };
			RightEar.Accel_ = 0.0f;
			RightEar.MoveFlagR = false;
		}
	}
	if (LeftEar.MoveFlagR)
	{
		//左の耳
		if (LeftEar.pos.x <= CenterEarPos_X &&
			LeftEar.pos.y <= CenterEarPos_Y)
		{
			LeftEar.pos += LeftEar.vel;
			LeftEar.Accel_ += EarAccel;
			Position2 Lrad = { CenterEarPos_X - LeftEarPos_X  , CenterEarPos_Y - LREarPos_Y };
			LeftEar.vel = Lrad.Normalized() * (EarSpeed + LeftEar.Accel_);
		}
		else
		{
			LeftEar.pos = { CenterEarPos_X,CenterEarPos_Y };
			LeftEar.Accel_ = 0.0f;
			LeftEar.MoveFlagR = false;
		}
	}
	if (AddEar.MoveFlagR)
	{
		//追加の耳
		if (AddEar.pos.x <= LeftEarPos_X &&
			AddEar.pos.y <= LREarPos_Y)
		{
			AddEar.pos += AddEar.vel;
			AddEar.Accel_ += EarAccel;
			Position2 Arad = { LeftEarPos_X - LUear.x , LREarPos_Y - LUear.y };
			AddEar.vel = Arad.Normalized() * (EarSpeed * 2.0f + AddEar.Accel_);

		}
		else
		{
			AddEar.pos = { LeftEarPos_X,LREarPos_Y };
			AddEar.Accel_ = 0.0f;
			AddEar.MoveFlagR = false;
		}
	}
	if (Rflag_ &&
		!CenterEar.MoveFlagR &&
		!RightEar.MoveFlagR &&
		!LeftEar.MoveFlagR &&
		!AddEar.MoveFlagR)
	{
		ImageChangeF_ = true;
	}
}

void SelectScene::EarMoveL(void)
{
	if (CenterEar.MoveFlagL)
	{
		//真ん中の耳
		if (CenterEar.pos.x >= LeftEarPos_X &&
			CenterEar.pos.y >= LREarPos_Y)
		{
			CenterEar.pos += CenterEar.vel;
			CenterEar.Accel_ += EarAccel;
			Position2 Crad = { LeftEarPos_X - CenterEarPos_X, LREarPos_Y - CenterEarPos_Y };
			CenterEar.vel = Crad.Normalized() * (EarSpeed + CenterEar.Accel_);
		}
		else
		{
			CenterEar.pos = { LeftEarPos_X,LREarPos_Y };
			CenterEar.Accel_ = 0.0f;
			CenterEar.MoveFlagL = false;
		}
	}
	if (RightEar.MoveFlagL)
	{
		//右の耳
		if (RightEar.pos.x >= CenterEarPos_X &&
			RightEar.pos.y <= CenterEarPos_Y)
		{
			RightEar.pos += RightEar.vel;
			RightEar.Accel_ += EarAccel;
			Position2 Rrad = { CenterEarPos_X - RightEarPos_X  , CenterEarPos_Y - LREarPos_Y };
			RightEar.vel = Rrad.Normalized() * (EarSpeed + RightEar.Accel_);
		}
		else
		{
			RightEar.pos = { CenterEarPos_X,CenterEarPos_Y };
			RightEar.Accel_ = 0.0f;
			RightEar.MoveFlagL = false;
		}
	}
	if (LeftEar.MoveFlagL)
	{
		//左の耳 
		if (LeftEar.pos.x >= LUear.x &&
			LeftEar.pos.y >= LUear.y)
		{
			LeftEar.pos += LeftEar.vel;
			LeftEar.Accel_ += EarAccel;
			Position2 Lrad = { LUear.x - LeftEarPos_X  ,  LUear.y - LREarPos_Y };
			LeftEar.vel = Lrad.Normalized() * (EarSpeed + LeftEar.Accel_);
		}
		else
		{
			LeftEar.pos = { 0,-500 };
			LeftEar.Accel_ = 0.0f;
			LeftEar.MoveFlagL = false;
		}
	}
	if (AddEar.MoveFlagL)
	{
		//追加の耳
		if (AddEar.pos.x >= RightEarPos_X &&
			AddEar.pos.y <= LREarPos_Y)
		{
			AddEar.pos += AddEar.vel;
			AddEar.Accel_ += EarAccel;
			Position2 Arad = { RightEarPos_X - RUear.x , LREarPos_Y - RUear.y };
			AddEar.vel = Arad.Normalized() * (EarSpeed * 2.0f + AddEar.Accel_);
		}
		else
		{
			AddEar.pos = { RightEarPos_X,LREarPos_Y };
			AddEar.Accel_ = 0.0f;
			AddEar.MoveFlagL = false;
		}
	}
	if (Lflag_ &&
		!CenterEar.MoveFlagL &&
		!RightEar.MoveFlagL &&
		!LeftEar.MoveFlagL &&
		!AddEar.MoveFlagL)
	{
		ImageChangeF_ = true;
	}
}

//--------------------------------------------------------------------------
//●グラフィックの描画
//--------------------------------------------------------------------------
void SelectScene::Draw(void)
{
	DrawGraph(0, 0, selectbg_, false);
	DrawGraph(0, 0, selectLogoBW_, true);

	DrawGraph(60, 480 - ArrowSizeY, LeftArrow_, true);
	DrawGraph(720 - ArrowSizeX, 480 - ArrowSizeY,RightArrow_, true);
	DrawGraph(390 - StartSizeX/2, 480 - StartSizeY, StartButton_, true);

	if ((blinkCnt_ / 20) % 2)
	{
		DrawGraph(0, 0, selectLogoBO_, true);
	}

	if (Rflag_ || Lflag_)
	{
		if (pageCnt_ > 1)
		{
			DrawGraph(LeftEar.pos.x, LeftEar.pos.y, lpSceneCtl.mimiImage_[(pageCnt_ - 2)], true);	//ひだり
		}
		if (pageCnt_ > 2&&Rflag_)
		{
			DrawGraph(AddEar.pos.x, AddEar.pos.y, lpSceneCtl.mimiImage_[(pageCnt_ - 3)], true);	//右移動時の追加分
		}
		DrawGraph(CenterEar.pos.x, CenterEar.pos.y, lpSceneCtl.mimiImage_[(pageCnt_ - 1)], true);	//真ん中
		if (pageCnt_ < static_cast<int>(CHARACTER::MAX))
		{
			DrawGraph(RightEar.pos.x, RightEar.pos.y, lpSceneCtl.mimiImage_[(pageCnt_)], true);	//みぎ
		}
		if (pageCnt_ < static_cast<int>(CHARACTER::MAX)-1&&Lflag_)
		{
			DrawGraph(AddEar.pos.x, AddEar.pos.y, lpSceneCtl.mimiImage_[(pageCnt_ + 1)], true);	//左移動時の追加分
		}
	}
	else
	{
		LeftEar.pos = { LeftEarPos_X, LREarPos_Y };
		CenterEar.pos = { CenterEarPos_X, CenterEarPos_Y };
		RightEar.pos = { RightEarPos_X,LREarPos_Y };
		AddEar.pos = { -EarSizeX,-EarSizeY };
		if (pageCnt_ > 1)
		{
			DrawGraph(LeftEar.anotherpos.x, LeftEar.anotherpos.y, lpSceneCtl.mimiImage_[(pageCnt_ - 2)], true);	//ひだり
		}
		DrawGraph(CenterEar.anotherpos.x, CenterEar.anotherpos.y, lpSceneCtl.mimiImage_[(pageCnt_ - 1)], true);	//真ん中
		if (pageCnt_ < static_cast<int>(CHARACTER::MAX))
		{
			DrawGraph(RightEar.anotherpos.x, RightEar.anotherpos.y, lpSceneCtl.mimiImage_[(pageCnt_)], true);	//みぎ
		}
	}

}

//--------------------------------------------------------------------------
//●初期化
//--------------------------------------------------------------------------
bool SelectScene::Init(void)
{
	TRACE("SelectSceneの初期化\n");

	pageCnt_ = 3;

	//耳の位置の初期化
	LeftEar.pos = { LeftEarPos_X, LREarPos_Y };
	CenterEar.pos = { CenterEarPos_X, CenterEarPos_Y };
	RightEar.pos = { RightEarPos_X,LREarPos_Y };
	AddEar.pos = { -EarSizeX,-EarSizeY };

	LeftEar.anotherpos = { LeftEarPos_X, LREarPos_Y };
	CenterEar.anotherpos = { CenterEarPos_X, CenterEarPos_Y };
	RightEar.anotherpos = { RightEarPos_X,LREarPos_Y };
	AddEar.anotherpos = { -EarSizeX,-EarSizeY };


	ImageChangeF_ = false;
	//画像ロード
	selectbg_ = LoadGraph(GH_BG_SELECT);
	selectLogoBO_ = LoadGraph(GH_LOGO_SELECT_BO);
	selectLogoBW_ = LoadGraph(GH_LOGO_SELECT_BW);

	RightArrow_ = LoadGraph(GH_ICON_RIGHTARROW);
	LeftArrow_ = LoadGraph(GH_ICON_LEFTARROW);
	StartButton_ = LoadGraph(GH_ICON_START);

	Select_SE_ = LoadSoundMem(SH_SelectSE);
	atfSE_ = LoadSoundMem(SH_ATF);

	return true;
}

//--------------------------------------------------------------------------
//●リソースの開放
//--------------------------------------------------------------------------
bool SelectScene::Release(void)
{
	TRACE("SelectSceneの解放\n");
	DeleteGraph(selectbg_);
	DeleteGraph(selectLogoBO_);
	DeleteGraph(selectLogoBW_);
	return true;
}
