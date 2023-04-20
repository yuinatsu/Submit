#include <DxLib.h>
#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../Obj/Player.h"
#include "../common/ImageMng.h"
#include "Transition/CrossOver.h"
#include "../common/FilePass.h"
#include "TitleScene.h"

SelectScene::SelectScene()
{
	Init();
	DrawOwnScreen(0.0f);
}

SelectScene::~SelectScene()
{
}

bool SelectScene::Init(void)
{
	tmxObj_ = std::make_shared<TmxObj>();
	// ステージ情報
	tmxObj_->LoadTmx("Tiled/selectMap.tmx");
	selectBGM_ = LoadSoundMem(SH_BGM_SELECT);
	selectSE_ = LoadSoundMem(SH_SE_SELECT);
	PlaySoundMem(selectBGM_, DX_PLAYTYPE_LOOP);

	// ステージナンバー
	stageNum_ = 1;

	count_ = 0;
	startCnt_ = 0;
	playerFlag_ = true;
	logoFlag_ = true;

	lpImageMng.GetID(GH_LOGO_SELECT, "select");

	return true;
}

uniqueBaseScene SelectScene::Update(double delta, uniqueBaseScene own)
{
	for (const auto& obj : obj_)
	{
		// プレイヤーの更新
		obj->Update(delta);
		// 重力更新
		obj->GravUpdate(delta);
		// ステージセレクト
		selectFlag_ = obj->GetSelectFlag();
		// ポーズ画面
		pauseNum_ = obj->GetPauseNum();
	}

	if (count_ >= 2.8 && playerFlag_ == true)
	{
		obj_.emplace_back(std::make_unique<Player>(CntType::Key, tmxObj_, Vector2F(32, 608)));
		lpSceneMng.SetCntType(CntType::Key);
		playerFlag_ = false;
	}

	if (pauseNum_ == 1)
	{
		// ブザー音
	}
	if (pauseNum_ == 2)
	{
		// 色情報リセット
		lpColorMng.Init();
		SetBackgroundColor(0, 0, 0);
		StopSoundMem(selectBGM_);
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<TitleScene>());
	}
	if (pauseNum_ == 3)
	{
		// ブザー音
	}
	if (pauseNum_ == 4)
	{

	}

	count_ += delta;
	if (count_ >= 1.0)
	{
		logoFlag_ = false;
	}

	// ステージ決定
	if (selectFlag_)
	{
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		StopSoundMem(selectBGM_);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<GameScene>());
	}
	DrawOwnScreen(delta);
	return std::move(own);
}

void SelectScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClearDrawScreen();

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
							if (count_ > 1.3 && (tmxObj_->GetMapData("black", x, y) || tmxObj_->GetMapData("door", x, y)))
							{
								DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
							}
							else
							{
								if (count_ > 1.5 && tmxObj_->GetMapData("light blue", x, y))			// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 1.7 && tmxObj_->GetMapData("purple", x, y))				// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 1.9 && tmxObj_->GetMapData("yellow", x, y))				// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.1 && tmxObj_->GetMapData("green", x, y))				// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.3 && tmxObj_->GetMapData("red", x, y))					// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.5 && tmxObj_->GetMapData("orange", x, y))				// カウントを一定数加算
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (!logoFlag_)
								{
									startCnt_++;
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

	auto alpha = static_cast<int>(255.0 * count_ / 1.0);			// 255(α値のMAX) : 3.0(クロスオーバーの時間) = alpha(現在時間のα値) : count(処理開始からの経過時間)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(0, 0, lpImageMng.GetID("select")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);

	for (const auto& obj : obj_)
	{
		obj->Draw();
	}
}
