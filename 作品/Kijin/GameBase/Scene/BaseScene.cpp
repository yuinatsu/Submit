#include <DxLib.h>
#include "BaseScene.h"
#include "../SceneManager.h"
#include "../Object/ObjectManager.h"
#include "../Common/ResourceMng.h"
#include "../Common/Camera.h"
#include "../UI/UiManager.h"
#include "../Shader/PEManager.h"
#include "../Common/Input/InputType/Keyboard.h"
#include "../Common/Input/InputType/Pad.h"
#include "../Common/Debug.h"
BaseScene::BaseScene(ScreenID id, SceneID sceneID) :
	scID_{ id }, nextID_{ sceneID }
{
	// �X�N���[���쐬
	lpSceneMng.GetResourceMng().MakeRenderTarget(screenHandle_,scID_, lpSceneMng.screenSize_<float>, true);
	
	// �J�����̍쐬
	camera_ = std::make_unique<Camera>();
}

BaseScene::BaseScene(std::unique_ptr<ObjectManager>&& objectManager, ScreenID id, SceneID sceneID) :
	scID_{ id }, nextID_{ sceneID }, objMng_{std::move(objectManager)}
{
	// �X�N���[���쐬
	lpSceneMng.GetResourceMng().MakeRenderTarget(screenHandle_, scID_, lpSceneMng.screenSize_<float>, true);

	// �J�����̍쐬
	camera_ = std::make_unique<Camera>();
}

BaseScene::~BaseScene()
{
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

void BaseScene::Back(void)
{
	// �f�t�H���g�͉������Ȃ�
}

BaseScene::SceneUptr BaseScene::Update(SceneUptr own, float delta, Controller& controller)
{
	Update(delta, controller);
	if (nextID_ != GetID())
	{
		
		// ���̃V�[����ID�ƈ�v���ĂȂ��Ƃ�
		if (sceneMakeFuncMap_.contains(nextID_))
		{
			// �쐬�pfunction������Ƃ���������s����
			return sceneMakeFuncMap_[nextID_](std::move(own));
		}
	}
	return own;
}

void BaseScene::ChangeSceneID(SceneID id)
{
	nextID_ = id;
}



void BaseScene::SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id)
{
	sceneMakeFuncMap_.emplace(id, std::move(sceneMakeFunc));
}

void BaseScene::Loaded(Controller& controller)
{
	// ���[�h�I�������̂Ń��X�g�̒��g�����ׂĎ��s����
	std::for_each(loadedFunc_.begin(), loadedFunc_.end(), [&controller](LoadedFunc& f) { f(controller); });

	// �����g��Ȃ��̂ŃN���A���Ƃ�
	loadedFunc_.clear();
}

void BaseScene::AddLoadedFunc(LoadedFunc&& loadedFunc)
{
	loadedFunc_.emplace_back(std::move(loadedFunc));
}





