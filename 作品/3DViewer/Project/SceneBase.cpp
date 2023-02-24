#include "SceneBase.h"
#include "SceneManager.h"

SceneBase::SceneBase(SceneManager* manager)
{
	sceneMng_ = manager;
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

SceneManager* SceneBase::GetSceneManager(void)
{
	return sceneMng_;
}
