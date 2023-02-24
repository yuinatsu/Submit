#include <DxLib.h>
#include "GameScene.h"
#include "EditScene.h"
#include "ClearScene.h"
#include "transition/FadeIo.h"
#include "MapCtl.h"
#include "SceneMng.h"
#include "Obj/Unit.h"
#include "../_debug/_DebugConOut.h"
#include "transition/FadeSwap.h"


GameScene::GameScene()
{
	//TRACE("GameScene�̐���\n");
	Init();				// �J�����̏��������ɂ����ق�������
}


GameScene::~GameScene()
{
	//TRACE("GameScene�̔j��\n");
}


uniqueBaseScene GameScene::Update(uniqueBaseScene own)
{
	//TRACE("GameScene�̍X�V\n");

	for (auto& obj : objList_)				// �I�u�W�F�N�g����͈�for��
	{
		// �v���C���[
		if (obj->GetObjType() == ObjType::Player)			// Map�̎��@�̃I�u�W�F�N�g�L��
		{
			// ���A����
			//-----------
			if (obj->GetState() == State::Ghost)			// �X�e�[�^�X�������҂��ł���΃f�t�H���g���W�Ɉړ�
			{
				obj->SetPos(obj->GetDefPos());				// �f�t�H���g���W���Z�b�g
				obj->SetState(State::Fall);					// �󒆂���̏ꍇ���l���ăX�e�[�^�X�͗����ɂ���
			}

			// �N���A�V�[���ڍs���f
			//-----------------------
			if (obj->GetPos().y <= -lpSceneMng.chipSize_.y)								// ���@��Y���W����ʏ㕔�ɍs������t�F�[�h�C���A�E�g���s���N���A�V�[���Ɉڍs
			{
				MapCtl::Destroy();
				return std::make_unique<FadeIo>(std::move(own), std::make_unique<ClearScene>());
			}
		}
		// �G
		if (obj->GetObjType() == ObjType::Enemy)			// Map�̎��@�̃I�u�W�F�N�g�L��
		{
			// ���A����
			//-----------
			if (obj->GetState() == State::Ghost)			// �X�e�[�^�X�������҂��ł���΃f�t�H���g���W�Ɉړ�
			{
				auto respawn = [](Vector2 pos) {
					//auto pos = obj->GetPos();					// ���S�������W���擾
					pos.y = 0;									// ���S�ʒu�̐^��ɍ��W�ύX

					for (int y = 0; y < lpSceneMng.worldArea_.y; y++)
					{
						for (int x = 0; x < lpSceneMng.worldArea_.x; x++)
						{
							if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::BLANK)			// �����ʒu�Ƀu���b�N������΂P�}�X���炷
							{
								pos.x += lpSceneMng.chipSize_.x;
								pos.x %= (lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x);

								// �ʉ�
								/*if (pos.x >= lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x)
								{
									pos.x = 0;
								}*/
							}
							else
							{
								return pos;
							}
						}
						pos.y += lpSceneMng.chipSize_.y;
					}
					return pos;					// �ꍇ�ɂ���Ă͕ǂ̒��Ŏ��ɑ�����
				};

				//auto pos = obj->GetPos();					// ���S�������W���擾
				//pos.y = 0;									// ���S�ʒu�̐^��ɍ��W�ύX
				//for (int y = 0; y < lpSceneMng.worldArea_.y; y++)
				//{
				//	for (int x = 0; x < lpSceneMng.worldArea_.x; x++)
				//	{
				//		if (lpMapCtl.GetMapData(ChipGP::BG, pos) != ChipID::BLANK)			// �����ʒu�Ƀu���b�N������΂P�}�X���炷
				//		{
				//			pos.x += lpSceneMng.chipSize_.x;
				//			if (pos.x >= lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x)
				//			{
				//				pos.x = 0;
				//			}
				//			// �ʉ�
				//			//pos.x %= (lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x);
				//			lpSceneMng.worldArea_.x* lpSceneMng.chipSize_.x;
				//		}
				//		else
				//		{
				//			break;
				//		}
				//	}
				//	pos.y += lpSceneMng.chipSize_.y;
				//}

				obj->SetPos(respawn(obj->GetPos()));							// ���S�ʒu�̐^����Z�b�g
				obj->SetState(State::Respawn);					// �󒆂���̏ꍇ���l���ăX�e�[�^�X�͗����ɂ���
			}
		}
	}
	
	if (ObjUpdate())
	{
		MapCtl::Destroy();
		return std::make_unique<EditScene>();				// EditScene��make_unique����
	}
	lpMapCtl.DigUpdate();				// ���@��̍X�V
	DrawOwnScreen();
	return std::move(own);
}


SceneID GameScene::GetSceneID()
{
	return SceneID::GAME;
}


void GameScene::DrawOwnScreen()
{
	SetDrawScreen(gameScrID_);
	ClsDrawScreen();
	// �Q�[���V�[���̓��e�̕`��
	lpMapCtl.DrawFunc();
	ObjDraw();

	SetDrawScreen(screenScrID_);					// scene�p�̊i�[�ꏊ�ɕ`��
	ClsDrawScreen();								// �`�悵����ʂ��N���A
	// �V�[���̓��e�̕`������s
	DrawGraph(lpSceneMng.drawOffset_.x, lpSceneMng.drawOffset_.y, gameScrID_, true);					// �Q�[����ʃI�t�Z�b�g�`��
}


bool GameScene::Init(void)
{
	MapCtl::Create(SceneID::GAME);


	// �J�����ݒ�
	//-------------
	camera_->InitArea();
	//camera_->SetPos(camera_->GetPos());				// �J�����̍��W���Z�b�g
	lpMapCtl.SetCamera(camera_);


	auto gameScrSize = lpSceneMng.viewArea_ * lpSceneMng.chipSize_;
	gameScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// ���W + 1 �ɂ��Ȃ��Ɛ��Ɣ��

	lpMapCtl.MapLoad("mapdata.txt", *this);		// EditScene�̃}�b�v�����󂯎��

	DrawOwnScreen();

	return true;
}
