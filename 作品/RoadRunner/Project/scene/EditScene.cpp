#include <DxLib.h>
#include <functional>
#include "SceneMng.h"
#include "EditScene.h"
#include "GameScene.h"
#include "MapCtl.h"
#include "../_debug/_DebugConOut.h"
#include "transition/CrossOver.h"


EditScene::EditScene()
{
	//TRACE("EditScene�̐���\n");
	Init();
}


EditScene::~EditScene()
{
	//TRACE("EditScene�̔j��\n");
}


uniqueBaseScene EditScene::Update(uniqueBaseScene own)
{
	//TRACE("EditScene�̍X�V\n");
	if (ObjUpdate())
	{
		MapCtl::Destroy();
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<GameScene>());
	}
	camera_->Update();
	DrawOwnScreen();
	return std::move(own);
}


SceneID EditScene::GetSceneID()
{
	return SceneID::EDIT;
}


void EditScene::DrawOwnScreen()
{
	SetDrawScreen(gameScrID_);
	ClsDrawScreen();
	// �Q�[���V�[���̓��e�̕`��
	lpMapCtl.DrawFunc();
	camera_->Draw();
	ObjDraw();

	SetDrawScreen(screenScrID_);					// scene�p�̊i�[�ꏊ�ɕ`��
	ClsDrawScreen();								// �`�悵����ʂ��N���A
	// �V�[���̓��e�̕`������s
	DrawGraph(drawOffset_.x, drawOffset_.y, gameScrID_, true);					// �Q�[����ʃI�t�Z�b�g�`��
	DrawOffsetScr();
}


void EditScene::DrawOffsetScr()
{
	lpMapCtl.DrawEditOffset();			// ���E���

	DrawString(drawOffset_.x, drawOffset_.y / 3, "F4 : �w���v", 0xffffff);
}


bool EditScene::Init()
{
	MapCtl::Create(SceneID::EDIT);

	viewArea_ = lpSceneMng.viewArea_;
	worldArea_ = lpSceneMng.worldArea_;
	chipSize_ = lpSceneMng.chipSize_;
	drawOffset_ = lpSceneMng.drawOffset_;

	auto gameScrSize = viewArea_ * chipSize_;
	gameScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// ���W + 1 �ɂ��Ȃ��Ɛ��Ɣ��

	auto itrObj = AddObjList(std::make_shared<EditCur>(camera_));


	// �J�����ݒ�
	//-------------
	//camera_->SetTarget((*itrObj));				// AddObjList(std::make_shared<EditCur>(camera_)��camera_���A�����ꂽ���Ȃ��̂ŏ����Ă���
	camera_->InitArea();
	camera_->SetPos(camera_->GetPos());				// �J�����̍��W���Z�b�g
	lpMapCtl.SetCamera(camera_);


	DrawOwnScreen();
	return true;
}
