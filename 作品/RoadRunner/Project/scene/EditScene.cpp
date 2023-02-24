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
	//TRACE("EditSceneの生成\n");
	Init();
}


EditScene::~EditScene()
{
	//TRACE("EditSceneの破棄\n");
}


uniqueBaseScene EditScene::Update(uniqueBaseScene own)
{
	//TRACE("EditSceneの更新\n");
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
	// ゲームシーンの内容の描画
	lpMapCtl.DrawFunc();
	camera_->Draw();
	ObjDraw();

	SetDrawScreen(screenScrID_);					// scene用の格納場所に描画
	ClsDrawScreen();								// 描画した画面をクリア
	// シーンの内容の描画を実行
	DrawGraph(drawOffset_.x, drawOffset_.y, gameScrID_, true);					// ゲーム画面オフセット描画
	DrawOffsetScr();
}


void EditScene::DrawOffsetScr()
{
	lpMapCtl.DrawEditOffset();			// 左右矢印

	DrawString(drawOffset_.x, drawOffset_.y / 3, "F4 : ヘルプ", 0xffffff);
}


bool EditScene::Init()
{
	MapCtl::Create(SceneID::EDIT);

	viewArea_ = lpSceneMng.viewArea_;
	worldArea_ = lpSceneMng.worldArea_;
	chipSize_ = lpSceneMng.chipSize_;
	drawOffset_ = lpSceneMng.drawOffset_;

	auto gameScrSize = viewArea_ * chipSize_;
	gameScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// 座標 + 1 にしないと線と被る

	auto itrObj = AddObjList(std::make_shared<EditCur>(camera_));


	// カメラ設定
	//-------------
	//camera_->SetTarget((*itrObj));				// AddObjList(std::make_shared<EditCur>(camera_)でcamera_が連動されたくないので消しておく
	camera_->InitArea();
	camera_->SetPos(camera_->GetPos());				// カメラの座標をセット
	lpMapCtl.SetCamera(camera_);


	DrawOwnScreen();
	return true;
}
