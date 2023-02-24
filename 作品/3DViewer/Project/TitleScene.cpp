#include "DxLib.h"
#include "KeyCheck.h"
#include "SceneManager.h"
#include "Stage.h"
#include "Unit.h"
#include "RollBall.h"
#include "Enemy.h"
#include "Camera.h"
#include "AsoUtility.h"
#include "TitleScene.h"

TitleScene::TitleScene(SceneManager* manager) : SceneBase(manager)
{
}

void TitleScene::Init(void)
{
	stage_ = new Stage(sceneMng_);
	stage_->Init();

	unit_ = new Unit(sceneMng_);
	unit_->Init();

	rollBall_ = new RollBall(sceneMng_, unit_);
	rollBall_->Init();

	enemy_ = new Enemy(sceneMng_, unit_);
	enemy_->Init();

	sceneMng_->GetCamera()->SetUnit(unit_);
}

void TitleScene::Update(void)
{

	if (keyTrgDown[KEY_SYS_START])
	{
		sceneMng_->ChangeScene(SCENE_ID::TITLE, true);
	}
	stage_->Update();
	unit_->Update();
	rollBall_->Update();
	enemy_->Update();
}

void TitleScene::Draw(void)
{
	stage_->Draw();
	unit_->Draw();
	rollBall_->Draw();
	enemy_->Draw();
	DrawDebug();
}

void TitleScene::DrawDebug(void)
{
	Camera* camera = sceneMng_->GetCamera();

	VECTOR pos = camera->GetPos();
	VECTOR angles = camera->GetAngles();
	DrawFormatString(0, 10, 0xffffff, "�J�������W�@�F�@(%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 30, 0xffffff, "�J�����p�x�@�F�@(%.1f, %.1f, %.1f)", AsoUtility::Rad2DegD(angles.x), AsoUtility::Rad2DegD(angles.y), AsoUtility::Rad2DegD(angles.z));

	pos = camera->GetTargetPos();
	DrawFormatString(0, 50, 0xffffff, "�����_�@�F�@(%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);

	DrawSphere3D(pos, 10.0f, 5, 0xffffff, 0xffffff, true);

	pos = unit_->GetPos();
	angles = unit_->GetAngles();
	DrawFormatString(0, 90, 0xffffff, "�L�������W�@�F�@(%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 110, 0xffffff, "�L�����p�x�@�F�@(%.1f, %.1f, %.1f)", AsoUtility::Rad2DegD(angles.x), AsoUtility::Rad2DegD(angles.y), AsoUtility::Rad2DegD(angles.z));

	// 2�̍��W����p�x�����߂�
	VECTOR uPos = unit_->GetPos();
	VECTOR bPos = rollBall_->GetPos();

	float diffX = bPos.x - uPos.x;
	float diffZ = bPos.z - uPos.z;

	// Z�̐��������猩���p�x------------
	float rad = atan2(diffX, diffZ);
	float deg = AsoUtility::Rad2DegF(rad);

	DrawFormatString(0, 150, 0xffffff, "���{�[��");
	DrawFormatString(0, 170, 0xffffff, "���W�@�F�@(%.2f, %.2f, %.2f)", bPos.x, bPos.y, bPos.z);
	DrawFormatString(0, 190, 0xffffff, "�� Z�̐������@�F�@%.2f, ", deg);
	//----------------------------------

	// �L�����̕������猩�����̂̊p�x
	VECTOR uAngles = unit_->GetAngles();
	float viewRad = rad - uAngles.y;
	float viewDeg = AsoUtility::DegIn360(AsoUtility::Deg2RadF(viewRad));
	DrawFormatString(0, 210, 0xffffff, "�� ���j�b�g�p�x�@�F�@%.2f", viewDeg);

}

void TitleScene::Release(void)
{
	stage_->Release();
	delete stage_;

	unit_->Release();
	delete unit_;

	rollBall_->Release();
	delete rollBall_;

	enemy_->Release();
	delete enemy_;
}
