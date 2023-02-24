#include <DxLib.h>
#include "BaseScene.h"
#include "../Application.h"
#include "../Object/ObjectManager.h"
#include "../Common/ResourceMng.h"
#include "../Common/Camera.h"
#include "../UI/UiManager.h"
#include "../Common/Input/InputType/Keyboard.h"
#include "../Common/Input/InputType/Pad.h"
#include "../Common/Debug.h"
BaseScene::BaseScene(ScreenID id, SceneID sceneID) :
	scID_{ id }, nextID_{ sceneID }, isDrawBackScene_{ true }, isUpdateBackScene_{ false }
{
	// スクリーン作成
	lpResourceMng.MakeRenderTarget(screenHandle_,scID_, lpApp.screenSize_<float>, true);
	
	camera_ = std::make_unique<Camera>();
	// コントローラの情報
}

BaseScene::~BaseScene()
{
}

void BaseScene::Update(float delta, Controller& controller)
{
	if (isUpdateBackScene_)
	{
		if (back_)
		{
			back_->Update(delta, controller);
		}
	}
}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, *screenHandle_,true);
}

void BaseScene::Relese(void)
{
}

bool BaseScene::IsLoaded(void)
{
	return GetASyncLoadNum() <= 0;
}

BaseScene::SceneUptr BaseScene::Update(SceneUptr own, float delta, Controller& controller)
{
	Update(delta, controller);
	if (nextID_ != GetID())
	{
		if (front_.size() != 0U)
		{
			if (front_.contains(nextID_))
			{
				// 前に当たるシーンの時現在のシーンを後ろにセットする
				front_[nextID_]->SetBackScene(std::move(own));
				front_[nextID_]->ChangeBackFront();
				return std::move(front_[nextID_]);
			}
		}
		
		// 次のシーンのIDと一致してないとき
		if (sceneMakeFuncMap_.contains(nextID_))
		{
			// 作成用functionがあるときそれを実行する
			return sceneMakeFuncMap_[nextID_](std::move(own));
		}
		if (back_)
		{
			if (back_->GetID() == nextID_)
			{
				back_->SetFrontScene(std::move(own));
				back_->ChangeBackFront();
				return std::move(back_);
			}
		}
	}
	return own;
}

void BaseScene::ChangeSceneID(SceneID id)
{
	nextID_ = id;
}

void BaseScene::ChangeBackScene(void)
{
	if (back_)
	{
		nextID_ = back_->GetID();
	}
}

void BaseScene::SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id)
{
	sceneMakeFuncMap_.emplace(id, std::move(sceneMakeFunc));
}

void BaseScene::Loaded(Controller& controller)
{
	std::for_each(loadedFunc_.begin(), loadedFunc_.end(), [&controller](LoadedFunc& f) { f(controller); });
	for (auto& scene : front_)
	{
		scene.second->Loaded(controller);
	}
}

void BaseScene::AddLoadedFunc(LoadedFunc&& loadedFunc)
{
	loadedFunc_.emplace_back(std::move(loadedFunc));
}

void BaseScene::SetBackScene(SceneUptr back)
{
	back_ = std::move(back);
}


void BaseScene::SetFrontScene(SceneUptr front)
{
	front_[front->GetID()] = std::move(front);
}

void BaseScene::ChangeBackFront(void)
{
	nextID_ = GetID();
}

void BaseScene::DrawSceneBackScene(void)
{
	if (isUpdateBackScene_ && isDrawBackScene_)
	{
		if (back_)
		{
			back_->DrawScene();
		}
	}

}

void BaseScene::DrawBackScene(void)
{
	if (isDrawBackScene_)
	{
		if (back_)
		{
			back_->Draw();
		}
	}
}


