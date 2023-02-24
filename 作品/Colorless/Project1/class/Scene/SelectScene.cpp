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
	// �X�e�[�W���
	//tmxObj_->LoadTmx("Tiled/select.tmx");
	tmxObj_->LoadTmx("Tiled/selectMap.tmx");
	selectBGM_ = LoadSoundMem(SH_BGM_SELECT);
	selectSE_ = LoadSoundMem(SH_SE_SELECT);
	PlaySoundMem(selectBGM_, DX_PLAYTYPE_LOOP);

	// �X�e�[�W�i���o�[
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
		// �v���C���[�̍X�V
		obj->Update(delta);
		// �d�͍X�V
		obj->GravUpdate(delta);
		// �X�e�[�W�Z���N�g
		selectFlag_ = obj->GetSelectFlag();
		// �|�[�Y���
		pauseNum_ = obj->GetPauseNum();
	}

	if (count_ >= 2.8 && playerFlag_ == true)
	{
		// �R���g���[���[���
		//if (GetJoypadNum())
		//{
		//	obj_.emplace_back(std::make_unique<Player>(CntType::Pad, tmxObj_, Vector2F(32, 608)));
		//	lpSceneMng.SetCntType(CntType::Pad);
		//}
		//else
		//{
			obj_.emplace_back(std::make_unique<Player>(CntType::Key, tmxObj_, Vector2F(32, 608)));
			lpSceneMng.SetCntType(CntType::Key);
		//}
		playerFlag_ = false;
	}

	if (pauseNum_ == 1)
	{
		// �u�U�[��
	}
	if (pauseNum_ == 2)
	{
		// �F��񃊃Z�b�g
		lpColorMng.Init();
		SetBackgroundColor(0, 0, 0);
		StopSoundMem(selectBGM_);
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<TitleScene>());
	}
	if (pauseNum_ == 3)
	{
		// �u�U�[��
	}
	if (pauseNum_ == 4)
	{

	}

	count_ += delta;
	if (count_ >= 1.0)
	{
		logoFlag_ = false;
	}

	// �X�e�[�W����
	if (selectFlag_)
	{
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK);
		StopSoundMem(selectBGM_);
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<GameScene>());
	}
	//if ((lpSceneMng.keyBuf_[KEY_INPUT_X] && !lpSceneMng.keyBufOld_[KEY_INPUT_X]))
	//{
	//	tmxObj_->ClearMapData();
	//	tmxObj_->LoadTmx("Tiled/select.tmx");
	//}
	//if ((lpSceneMng.keyBuf_[KEY_INPUT_Z] && !lpSceneMng.keyBufOld_[KEY_INPUT_Z]))
	//{
	//	tmxObj_->ClearMapData();
	//	tmxObj_->LoadTmx("Tiled/selectMap.tmx");
	//}
	DrawOwnScreen(delta);
	return std::move(own);
}

void SelectScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClearDrawScreen();

	// �X�e�[�W�\���o�����o
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
								if (count_ > 1.5 && tmxObj_->GetMapData("light blue", x, y))			// �J�E���g����萔���Z
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 1.7 && tmxObj_->GetMapData("purple", x, y))				// �J�E���g����萔���Z
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 1.9 && tmxObj_->GetMapData("yellow", x, y))				// �J�E���g����萔���Z
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.1 && tmxObj_->GetMapData("green", x, y))				// �J�E���g����萔���Z
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.3 && tmxObj_->GetMapData("red", x, y))					// �J�E���g����萔���Z
								{
									DrawGraph(x * tileSize.x, y * tileSize.y, lpImageMng.GetID("map")[gid], true);
								}
								if (count_ > 2.5 && tmxObj_->GetMapData("orange", x, y))				// �J�E���g����萔���Z
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
		// �X�e�[�W�\��
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

	auto alpha = static_cast<int>(255.0 * count_ / 1.0);			// 255(���l��MAX) : 3.0(�N���X�I�[�o�[�̎���) = alpha(���ݎ��Ԃ̃��l) : count(�����J�n����̌o�ߎ���)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(0, 0, lpImageMng.GetID("select")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);

	for (const auto& obj : obj_)
	{
		obj->Draw();
	}
}
