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
	//TRACE("GameSceneの生成\n");
	Init();				// カメラの初期化を先にしたほうがいい
}


GameScene::~GameScene()
{
	//TRACE("GameSceneの破棄\n");
}


uniqueBaseScene GameScene::Update(uniqueBaseScene own)
{
	//TRACE("GameSceneの更新\n");

	for (auto& obj : objList_)				// オブジェクト情報を範囲for文
	{
		// プレイヤー
		if (obj->GetObjType() == ObjType::Player)			// Mapの自機のオブジェクト有無
		{
			// 復帰判定
			//-----------
			if (obj->GetState() == State::Ghost)			// ステータスが復活待ちであればデフォルト座標に移動
			{
				obj->SetPos(obj->GetDefPos());				// デフォルト座標をセット
				obj->SetState(State::Fall);					// 空中からの場合を考えてステータスは落下にする
			}

			// クリアシーン移行判断
			//-----------------------
			if (obj->GetPos().y <= -lpSceneMng.chipSize_.y)								// 自機のY座標が画面上部に行ったらフェードインアウトを行いクリアシーンに移行
			{
				MapCtl::Destroy();
				return std::make_unique<FadeIo>(std::move(own), std::make_unique<ClearScene>());
			}
		}
		// 敵
		if (obj->GetObjType() == ObjType::Enemy)			// Mapの自機のオブジェクト有無
		{
			// 復帰判定
			//-----------
			if (obj->GetState() == State::Ghost)			// ステータスが復活待ちであればデフォルト座標に移動
			{
				auto respawn = [](Vector2 pos) {
					//auto pos = obj->GetPos();					// 死亡した座標を取得
					pos.y = 0;									// 死亡位置の真上に座標変更

					for (int y = 0; y < lpSceneMng.worldArea_.y; y++)
					{
						for (int x = 0; x < lpSceneMng.worldArea_.x; x++)
						{
							if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::BLANK)			// 復活位置にブロックがあれば１マスずらす
							{
								pos.x += lpSceneMng.chipSize_.x;
								pos.x %= (lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x);

								// 別解
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
					return pos;					// 場合によっては壁の中で死に続ける
				};

				//auto pos = obj->GetPos();					// 死亡した座標を取得
				//pos.y = 0;									// 死亡位置の真上に座標変更
				//for (int y = 0; y < lpSceneMng.worldArea_.y; y++)
				//{
				//	for (int x = 0; x < lpSceneMng.worldArea_.x; x++)
				//	{
				//		if (lpMapCtl.GetMapData(ChipGP::BG, pos) != ChipID::BLANK)			// 復活位置にブロックがあれば１マスずらす
				//		{
				//			pos.x += lpSceneMng.chipSize_.x;
				//			if (pos.x >= lpSceneMng.worldArea_.x * lpSceneMng.chipSize_.x)
				//			{
				//				pos.x = 0;
				//			}
				//			// 別解
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

				obj->SetPos(respawn(obj->GetPos()));							// 死亡位置の真上をセット
				obj->SetState(State::Respawn);					// 空中からの場合を考えてステータスは落下にする
			}
		}
	}
	
	if (ObjUpdate())
	{
		MapCtl::Destroy();
		return std::make_unique<EditScene>();				// EditSceneをmake_uniqueする
	}
	lpMapCtl.DigUpdate();				// 穴掘りの更新
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
	// ゲームシーンの内容の描画
	lpMapCtl.DrawFunc();
	ObjDraw();

	SetDrawScreen(screenScrID_);					// scene用の格納場所に描画
	ClsDrawScreen();								// 描画した画面をクリア
	// シーンの内容の描画を実行
	DrawGraph(lpSceneMng.drawOffset_.x, lpSceneMng.drawOffset_.y, gameScrID_, true);					// ゲーム画面オフセット描画
}


bool GameScene::Init(void)
{
	MapCtl::Create(SceneID::GAME);


	// カメラ設定
	//-------------
	camera_->InitArea();
	//camera_->SetPos(camera_->GetPos());				// カメラの座標をセット
	lpMapCtl.SetCamera(camera_);


	auto gameScrSize = lpSceneMng.viewArea_ * lpSceneMng.chipSize_;
	gameScrID_ = MakeScreen(gameScrSize.x + 1, gameScrSize.y + 1, 255);						// 座標 + 1 にしないと線と被る

	lpMapCtl.MapLoad("mapdata.txt", *this);		// EditSceneのマップ情報を受け取る

	DrawOwnScreen();

	return true;
}
