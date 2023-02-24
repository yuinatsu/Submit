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
	TRACE("ClearSceneの生成\n");
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
			if (inputData.count("Ａ"))
			{
				if (inputData.at("Ａ"))
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

	SetDrawScreen(screenScrID_);					// scene用の格納場所に描画
	ClsDrawScreen();								// 描画した画面をクリア
	// シーンの内容の描画を実行
	DrawGraph(lpSceneMng.drawOffset_.x, lpSceneMng.drawOffset_.y, clearScrID_, true);					// ゲーム画面オフセット描画
}


bool ClearScene::Init()
{
	MapCtl::Create(SceneID::CLEAR);

	lpMapCtl.SetCamera(camera_);


	auto gameScrSize = lpSceneMng.viewArea_ * lpSceneMng.chipSize_;
	clearScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// 座標 + 1 にしないと線と被る

	lpMapCtl.MapLoad("cleardata.txt", *this);					// クリアシーンのデータを読み込み

	DrawOwnScreen();

	return true;
}
