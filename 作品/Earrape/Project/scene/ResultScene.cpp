#define _CRT_SECURE_NO_WARNINGS
#include<DxLib.h>
#include<stdio.h>
#include "ResultScene.h"
#include"SceneCtl.h"
#include"../FilePass.h"
#include "../input/Mouse.h"

ResultScene::ResultScene() 
{
}

ResultScene::~ResultScene()
{
}

SCN_ID ResultScene::Update(char* keyData, char* keyDataOld)
{
	lpMouse.Update();
	//効果音鳴り終わり後に左クリックでゲームシーン、ＳＰＡＣＥを押せばタイトルシーンに移行
	if (rollSEflag_)
	{
		if (lpMouse.GetPresCnt(Mouse::LMB) == 1)
		{
			return SCN_ID::TITLE;
		}
	}
	//if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
	//{
	//	return SCN_ID::GAME;
	//}
	if (lpMouse.GetPresCnt(Mouse::RMB) == 1)
	{
		return SCN_ID::SELECT;
	}

	return SCN_ID::RESULT;
}

void ResultScene::Draw(void)
{
	DrawGraph(0, 0, BackGroundImage_, true);	//背景描画
	DrawResult();

	//吹き出しを画像に置き換え
	constexpr int SpeechBalloon_OffsetX = 15;
	constexpr int SpeechBalloon_OffsetY = 30;
	if (DrumCount_ < DrumLimit)
	{
		DrawGraph(lpSceneCtl.scnArea_.x - EarSizeX - SpeechBalloon_SizeX + SpeechBalloon_OffsetX,
			lpSceneCtl.scnArea_.y - SpeechBalloon_SizeY - SpeechBalloon_OffsetY,
			EarComment_[0], true);
	}
	else
	{
		DrawGraph(lpSceneCtl.scnArea_.x - EarSizeX - SpeechBalloon_SizeX + SpeechBalloon_OffsetX,
			lpSceneCtl.scnArea_.y - SpeechBalloon_SizeY - SpeechBalloon_OffsetY,
			EarComment_[CommentNum_], true);
	}
	DrawGraph(lpSceneCtl.scnArea_.x - EarSizeX, lpSceneCtl.scnArea_.y - EarSizeY, lpSceneCtl.mimiImage_[lpSceneCtl.charCnt_ - 1], true);
}

bool ResultScene::Init(void)
{
	DrumCount_ = 0;
	constexpr int ABCcntX = RankResult_Cnt;
	constexpr int ABCcntY = 1;
	LoadDivGraph(GH_LOGO_RANK_W, ABCcntX * ABCcntY, ABCcntX, ABCcntY, RankResult_SizeX, RankResult_SizeY, ABCsImage_);
	BackGroundImage_ = LoadGraph(GH_BG_RESULT);
	ScoreImage_ = LoadGraph(GH_LOGO_SCORE_BW);
	CommentNum_ = rnd() % 21 + 1;
	char FileName[100];
	EarComment_[0] = LoadGraph("./resource/img/comment/fukidasi.png");
	for (int n = 1; n < EarCommentCnt + 1; n++)
	{
		sprintf(FileName, "./resource/img/comment/EarComments_%d.png", n);
		EarComment_[n] = LoadGraph(FileName);
	}

	DrumrollSE_ = LoadSoundMem(Drum_roll_SE);
	PlaySoundMem(DrumrollSE_, DX_PLAYTYPE_BACK, true);
	roll_SE_ = LoadSoundMem(roll_SE);
	rollSEflag_ = false;
	return true;
}

bool ResultScene::Release(void)
{
	DeleteGraph(ScoreImage_);
	DeleteGraph(RankImage_);
	DeleteGraph(BackGroundImage_);
	for (int n = 0; n < RankResult_Cnt; n++)
	{
		DeleteGraph(ABCsImage_[n]);
	}

	return true;
}

void ResultScene::DrawResult(void)
{
	DrawGraph(FontOffsetX - ScoreSizeX - FontNumSizeX, FontOffsetY - ScoreSizeY, ScoreImage_, true);

	//スコアの表示
	//STotalscore_が６桁未満の場合、０を挿入し６桁にする。
	if (DrumCount_ < DrumLimit)
	{
		//ドラムロールが鳴り終わるまでスコアをランダムで表示 
		for (int i = 0; i < ScoreDigit; i++)
		{
			DrawGraph(FontOffsetX + FontNumSizeX * i,
				FontOffsetY, lpSceneCtl.NumberFontImage[rnd() % 10], true);
		}
		DrumCount_++;
	}
	else
	{
		StopSoundMem(DrumrollSE_);
		if (!rollSEflag_)
		{
			PlaySoundMem(roll_SE_, DX_PLAYTYPE_BACK, true);
			rollSEflag_ = true;
		}
		std::string STotalscore_ = std::to_string(lpSceneCtl.TotalScore_);
		while (STotalscore_.length() != ScoreDigit)
		{
			STotalscore_.insert(0, "0");
		}
		for (int i = 0; i < ScoreDigit; i++)
		{
			int L = atoi(STotalscore_.substr(i, 1).c_str());	//残り時間の内、一桁を取り出す
			DrawGraph(FontOffsetX + FontNumSizeX * i,
				FontOffsetY, lpSceneCtl.NumberFontImage[L], true);
		}

		//ABCの表示
		//５００００点刻み（仮）
		constexpr int RankScore = 750000;
		if (0 <= lpSceneCtl.TotalScore_ && RankScore >= lpSceneCtl.TotalScore_)
		{
			DrawGraph(FontOffsetX,
				FontOffsetY + FontNumSizeY * 3 / 2, ABCsImage_[2], true);
		}
		else if (RankScore <= lpSceneCtl.TotalScore_ && RankScore * 2 >= lpSceneCtl.TotalScore_)
		{
			DrawGraph(FontOffsetX,
				FontOffsetY + FontNumSizeY * 3 / 2, ABCsImage_[1], true);
		}
		else
		{
			DrawGraph(FontOffsetX,
				FontOffsetY + FontNumSizeY * 3 / 2, ABCsImage_[0], true);
		}
	}
}
