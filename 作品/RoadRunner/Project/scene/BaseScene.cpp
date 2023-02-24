#include <DxLib.h>
#include "BaseScene.h"
#include "Obj/Obj.h"
#include "SceneMng.h"


BaseScene::BaseScene()
{
	screenScrID_ = MakeScreen(lpSceneMng.scnArea_.x, lpSceneMng.scnArea_.y, 255);					// スクリーンの作成
	camera_ = std::make_shared<Camera>();
	//auto tmp = objList_.begin();
}


BaseScene::~BaseScene()
{
}


uniqueBaseScene BaseScene::Update(uniqueBaseScene own)
{
	return uniqueBaseScene();
}


void BaseScene::Draw()
{
	DrawGraph(0, 0, screenScrID_, true);
	//DrawGraph((rand() % 5) - 2, (rand() % 5) - 2, screenScrID_, true);					// 画面揺れ（遊びで書いたやつ）
}


void BaseScene::DrawOwnScreen()
{
}


bool BaseScene::Init(void)
{
	return false;
}


ObjList::iterator BaseScene::AddObjList(ObjPtr&& objUni)
{
	objList_.push_back(std::move(objUni));				// objListに要素を追加				あらゆるオブジェクトの情報を集約している
	auto itr = objList_.end();							// objListのend()		itr ... iterator
	--itr;												// objListのend()の一個前の要素（要素的に言えば最後）	// 前演算
	return itr;											// return --objList_.end();		上の2行を統合した場合の文
}


bool BaseScene::ObjUpdate(void)
{
	bool flag = false;						// |= を使いたいので初期値を０（false）にする必要がある

	// 範囲for文
	//------------
	for (auto& data : objList_)
	{
		flag |= data->Update(objList_);				// |= ... 一回trueが立ったら後からfalseが来てもtrueのままになる		//flag = data->Update(); ... trueがあっても次にfalseが入ればtrueの情報がなくなるので今回は使えない

		// 別解
		//-------
		//if (data->Update())				// Obj::Updateの返り値がtrueだったらをflagにtrueを代入
		//{
		//	flag = true;
		//}
	}
	// itr自体がほしい場合はこちらを使用
	//------------------------------------
	//for (auto itr = objList_.begin(); itr != objList_.end(); ++itr)			// objのリストの中身を回すfor文
	//{
	//	flag |= (*itr)->Update();									// itrの中のその中に入る			// 別解 itr->get()->Update();
	//}

	if (camera_)							// カメラのエンプティチェック(無ければ入る)
	{
		camera_->Update();
	}
	return flag;
}


bool BaseScene::ObjDraw(void)
{
	// 範囲for文
	//------------
	for (auto& data : objList_)
	{
		data->Draw();
	}
	return true;
}
