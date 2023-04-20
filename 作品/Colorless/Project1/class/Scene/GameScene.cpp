#include <DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include "../Obj/Player.h"
#include "ClearScene.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "../common/ImageMng.h"
#include "Transition/CrossOver.h"
#include "../common/FileSize.h"
#include "../common/FilePass.h"


GameScene::GameScene()
{
	Init();
	DrawOwnScreen(0.0f);
}


GameScene::~GameScene()
{
}


bool GameScene::Init(void)
{
	tmxObj_ = std::make_shared<TmxObj>();
	// ステージ情報
	stage_ = lpSceneMng.GetStage();
	auto initPos = Vector2F();
	if (stage_ == Stage::Stage1)
	{
		stageNum_ = 1;
		initPos = Vector2F(60, 640);
		tmxObj_->LoadTmx("Tiled/map1.tmx");
		gameBGM1_ = LoadSoundMem(SH_BGM_GAME1);
		PlaySoundMem(gameBGM1_, DX_PLAYTYPE_LOOP);
	}
	else if (stage_ == Stage::Stage2)
	{
		stageNum_ = 2;
		initPos = Vector2F(60, 640);
		tmxObj_->LoadTmx("Tiled/map2.tmx");
		gameBGM2_ = LoadSoundMem(SH_BGM_GAME2);
		PlaySoundMem(gameBGM2_, DX_PLAYTYPE_LOOP);
	}
	else if (stage_ == Stage::Stage3)
	{
		stageNum_ = 3;
		initPos = Vector2F(60, 600);
		tmxObj_->LoadTmx("Tiled/map3.tmx");
		gameBGM3_ = LoadSoundMem(SH_BGM_GAME3);
		PlaySoundMem(gameBGM3_, DX_PLAYTYPE_LOOP);
	}
	else if (stage_ == Stage::Stage4)
	{
		stageNum_ = 4;
		initPos = Vector2F(0, 0);
		tmxObj_->LoadTmx("Tiled/map4.tmx");
		gameBGM1_ = LoadSoundMem(SH_BGM_GAME1);
		PlaySoundMem(gameBGM1_, DX_PLAYTYPE_LOOP);
	}
	else if (stage_ == Stage::Stage5)
	{
		stageNum_ = 5;
		initPos = Vector2F(60, 640);
		tmxObj_->LoadTmx("Tiled/map5.tmx");
		gameBGM2_ = LoadSoundMem(SH_BGM_GAME2);
		PlaySoundMem(gameBGM2_, DX_PLAYTYPE_LOOP);
	}
	else if (stage_ == Stage::Stage6)
	{
		stageNum_ = 6;
		initPos = Vector2F(60, 640);
		tmxObj_->LoadTmx("Tiled/map6.tmx");
		gameBGM3_ = LoadSoundMem(SH_BGM_GAME3);
		PlaySoundMem(gameBGM3_, DX_PLAYTYPE_LOOP);
	}

	selectSE_ = LoadSoundMem(SH_SE_SELECT);

	// アニメーション情報
	for (const auto& obj : objList_)
	{
		obj->SetAnimFlag(false);
	}

	// カウントダウン
	count_ = 0;
	callFlag_ = true;
	lpImageMng.GetID(GH_LOGO_GAME_CALL, "call", Vector2(lpSize.callImageSize_.x, lpSize.callImageSize_.y / lpSize.callDiv_.y), lpSize.callDiv_);
	lpImageMng.GetID(GH_ICON_GAME_RULE, "rule");

	clearCnt_ = 0;
	playerFlag_ = true;
	pauseNum_ = 0;

	return true;
}


uniqueBaseScene GameScene::Update(double delta, uniqueBaseScene own)
{
	for (const auto& obj : objList_)
	{
		// プレイヤーの更新
		obj->Update(delta);
		// 重力更新
		obj->GravUpdate(delta);
		// ボタンフラグ更新
		bflag_ = obj->GetBottanFlag();
		// カウントダウン時のアニメーション判定
		if (count_ >= CallCountMax * 2)
		{
			callFlag_ = false;
			obj->SetAnimFlag(true);
		}
		// クリア判定
		clearFlag_ = obj->GetClearFlag();
		// ポーズ画面
		pauseNum_ = obj->GetPauseNum();
	}


	if (count_ >= 1.5 && playerFlag_ == true)
	{
		objList_.emplace_back(std::make_unique<Player>(CntType::Key, tmxObj_, Vector2F(32, 608)));
		lpSceneMng.SetCntType(CntType::Key);
		playerFlag_ = false;
	}


	count_ += delta;

	if (clearFlag_)
	{
		if (clearCnt_ < 100)				// カウントを一定数加算
		{
			clearCnt_++;
		}
		else
		{
			// 色情報リセット
			lpColorMng.Init();
			SetBackgroundColor(0, 0, 0);
			switch (stageNum_)
			{
			case 1:
				StopSoundMem(gameBGM1_);
				break;
			case 2:
				StopSoundMem(gameBGM2_);
				break;
			case 3:
				StopSoundMem(gameBGM3_);
				break;
			case 4:
				StopSoundMem(gameBGM1_);
				break;
			case 5:
				StopSoundMem(gameBGM2_);
				break;
			case 6:
				StopSoundMem(gameBGM3_);
				break;
			default:
				break;
			}
			return std::make_unique<CrossOver>(std::move(own), std::make_unique<ClearScene>());
		}
	}

	// メニュー覧
	if (pauseNum_ == 1)
	{
		lpColorMng.Init();
		SetBackgroundColor(0, 0, 0);
		switch (stageNum_)
		{
		case 1:
			StopSoundMem(gameBGM1_);
			break;
		case 2:
			StopSoundMem(gameBGM2_);
			break;
		case 3:
			StopSoundMem(gameBGM3_);
			break;
		case 4:
			StopSoundMem(gameBGM1_);
			break;
		case 5:
			StopSoundMem(gameBGM2_);
			break;
		case 6:
			StopSoundMem(gameBGM3_);
			break;
		default:
			break;
		}
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<SelectScene>());
	}
	if (pauseNum_ == 2)
	{
		lpColorMng.Init();
		SetBackgroundColor(0, 0, 0);
		switch (stageNum_)
		{
		case 1:
			StopSoundMem(gameBGM1_);
			break;
		case 2:
			StopSoundMem(gameBGM2_);
			break;
		case 3:
			StopSoundMem(gameBGM3_);
			break;
		case 4:
			StopSoundMem(gameBGM1_);
			break;
		case 5:
			StopSoundMem(gameBGM2_);
			break;
		case 6:
			StopSoundMem(gameBGM3_);
			break;
		default:
			break;
		}
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<TitleScene>());
	}
	if (pauseNum_ == 3)
	{
		// 色情報リセット
		lpColorMng.Init();
		SetBackgroundColor(0, 0, 0);
		lpSceneMng.SetStage(stageNum_);
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<GameScene>());
	}
	if (pauseNum_ == 4)
	{

	}

	DrawOwnScreen(delta);
	return std::move(own);
}


void GameScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	// ステージ再更新
	if (bflag_)
	{
		tmxObj_->ClearMapData();
		if (stage_ == Stage::Stage1)
		{
			tmxObj_->LoadTmx("Tiled/map1.tmx");
		}
		else if(stage_ == Stage::Stage2)
		{
			tmxObj_->LoadTmx("Tiled/map2.tmx");
		}
		else if (stage_ == Stage::Stage3)
		{
			tmxObj_->LoadTmx("Tiled/map3.tmx");
		}
		else if (stage_ == Stage::Stage4)
		{
			tmxObj_->LoadTmx("Tiled/map4.tmx");
		}
		else if (stage_ == Stage::Stage5)
		{
			tmxObj_->LoadTmx("Tiled/map5.tmx");
		}
		else if (stage_ == Stage::Stage6)
		{
			tmxObj_->LoadTmx("Tiled/map6.tmx");
		}
	}

	// ステージ表示出現演出
	if (playerFlag_)
	{
		auto& tileSize = tmxObj_->GetTileSize();
		for (const auto& layer : tmxObj_->GetMapData())
		{
			for (int y = 0; y < tmxObj_->GetWorldArea().y; y++)
			{
				for (int x = 0; x < tmxObj_->GetWorldArea().x; x++)
				{
					if (static_cast<unsigned int>(y * static_cast<__int64>(tmxObj_->GetWorldArea().x) + x) < layer.second.size())
					{
						int gid = layer.second[y * static_cast<__int64>(tmxObj_->GetWorldArea().x) + x] - tmxObj_->GetFirstGid();
						if (gid >= 0)
						{
							auto flag = true;
							if (count_ > 0.5 && (tmxObj_->GetMapData("black", x, y) || tmxObj_->GetMapData("door", x, y)))
							{
								DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
							}
							else
							{
								if (count_ > 1.0)				// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// ステージ表示
		auto& tileSize = tmxObj_->GetTileSize();
		for (const auto& layer : tmxObj_->GetMapData())
		{
			for (int y = 0; y < tmxObj_->GetWorldArea().y; y++)
			{
				for (int x = 0; x < tmxObj_->GetWorldArea().x; x++)
				{
					if (static_cast<unsigned int>(y * static_cast<__int64>(tmxObj_->GetWorldArea().x) + x) < layer.second.size())
					{
						int gid = layer.second[y * static_cast<__int64>(tmxObj_->GetWorldArea().x) + x] - tmxObj_->GetFirstGid();
						if (gid >= 0)
						{
							DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
						}
					}
				}
			}
		}
	}

	// チュートリアル用
	if (stage_ == Stage::Stage1)
	{
		auto interval = 20;
		DrawGraph(lpSceneMng.GetScreenSize().x - lpSize.ruleSize_.x - interval, interval, lpImageMng.GetID("rule")[0], true);
	}

	// カウントダウン
	if (callFlag_)
	{
		CallDraw();							// ゲーム開始前のカウントダウン表示
	}

	// クリア判定
	if (clearFlag_)
	{
		DrawGraph((lpSceneMng.GetScreenSize().x - lpSize.callImageSize_.x) / 2, (lpSceneMng.GetScreenSize().y - lpSize.callImageSize_.y / lpSize.callDiv_.y) / 2, lpImageMng.GetID("call")[2], true);
	}

	for (const auto& obj : objList_)
	{
		obj->Draw();
		// ボタン情報をリセット
		obj->SetBottanFlag(false);
	}
}


void GameScene::CallDraw(void)
{
	// カウントダウン
	for (double cnt = 0; cnt < StartCallMax; cnt++)								// call.pngの上２枚分の表示
	{
		if (!(count_ >= CallCountMax + (cnt * CallCountMax)))					// カウントが一定数行ったら表示しないようにする
		{
			if (count_ >= 0.5 * (cnt + (cnt * 2.0)))								// callImageを一定数表示
			{
				DrawGraph((lpSceneMng.GetScreenSize().x - lpSize.callImageSize_.x) / 2, (lpSceneMng.GetScreenSize().y - lpSize.callImageSize_.y / lpSize.callDiv_.y) / 2, lpImageMng.GetID("call")[cnt], true);
			}
		}
	}
}