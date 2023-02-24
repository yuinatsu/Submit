#include <Dxlib.h>
#include "ClearScene.h"
#include "GameScene.h"
#include "EditScene.h"
#include "MapCtl.h"
#include "SceneMng.h"
#include "Obj/Unit.h"
#include "Input/ClearAuto.h"
#include "../_debug/_DebugConOut.h"


ClearScene::ClearScene()
{
	TRACE("ClearScene�̐���\n");
	Init();
}


ClearScene::~ClearScene()
{
}


uniqueBaseScene ClearScene::Update(uniqueBaseScene own)
{
	/*for (auto& obj : objList_)
	{
		if (obj->GetObjType() == ObjType::Player)
		{
			auto& inputData = input_->GetData(InputType::NOW);
			if (inputData.count("�`"))
			{
				if (inputData.at("�`"))
				{
					return std::make_unique<GameScene>();
				}
			}
		}
	}*/

	if (ObjUpdate())
	{
		MapCtl::Destroy();
		return std::make_unique<EditScene>();
	}
	DrawOwnScreen();
	return std::move(own);
}


SceneID ClearScene::GetSceneID()
{
	return SceneID::CLEAR;
}


void ClearScene::DrawOwnScreen()
{
	SetDrawScreen(clearScrID_);
	ClsDrawScreen();

	lpMapCtl.DrawFunc();
	ObjDraw();

	SetDrawScreen(screenScrID_);					// scene�p�̊i�[�ꏊ�ɕ`��
	ClsDrawScreen();								// �`�悵����ʂ��N���A
	// �V�[���̓��e�̕`������s
	DrawGraph(lpSceneMng.drawOffset_.x, lpSceneMng.drawOffset_.y, clearScrID_, true);					// �Q�[����ʃI�t�Z�b�g�`��
}


bool ClearScene::Init()
{
	MapCtl::Create(SceneID::CLEAR);

	lpMapCtl.SetCamera(camera_);


	auto gameScrSize = lpSceneMng.viewArea_ * lpSceneMng.chipSize_;
	clearScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// ���W + 1 �ɂ��Ȃ��Ɛ��Ɣ��

	lpMapCtl.MapLoad("cleardata.txt", *this);					// �N���A�V�[���̃f�[�^��ǂݍ���

	DrawOwnScreen();

	return true;
}
