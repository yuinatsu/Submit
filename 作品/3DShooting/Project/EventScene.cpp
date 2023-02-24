#include "EventScene.h"
#include "SceneManager.h"
#include "Camera.h"
#include "SpaceDome.h"
#include "BossShip.h"
#include "KeyCheck.h"

EventScene::EventScene(SceneManager* manager):
	SceneBase(manager)
{
}

void EventScene::Init(void)
{
	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FIXED);

	boss_ = new BossShip(mSceneManager, nullptr);
	boss_->Init();

	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();
}

void EventScene::Update(void)
{
	// ƒV[ƒ“‘JˆÚ
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
	}
	boss_->Update();
	mSpaceDome->Update();
}

void EventScene::Draw(void)
{
	mSpaceDome->Draw();
	boss_->Draw();
	
}

void EventScene::Release(void)
{
	boss_->Release();
	delete boss_;

	mSpaceDome->Release();
	delete mSpaceDome;
}
