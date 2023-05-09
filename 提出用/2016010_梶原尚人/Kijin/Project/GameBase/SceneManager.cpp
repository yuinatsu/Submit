#include "SceneManager.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "resource.h"
#include "Common/ResourceMng.h"
#include "Common/ThreadPool.h"
#include "Common/Input/InputConfig/InputConfig.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Common/Input/InputType/Keyboard.h"
#include "Common/Input/InputType/Pad.h"

// �|�[�Y��ʂ͊m�F�̂��߂���
#include "Scene/PauseScene.h"
#include "Scene/Transition/FadeLoading.h"

#include "Common/Debug.h"

SceneManager SceneManager::instance_;

int SceneManager::Run(void)
{
	if (!isInit_ || !Init())
	{
		return -1;
	}
	DebugLog("�������I��");
	while (!ProcessMessage() && !isEnd_)
	{
		// �X�V����
		Update();

		// �`�揈��
		Draw();
	}
	
	Effkseer_End();


	return 0;
}

void SceneManager::Draw()
{
	scene_->DrawScene();

	// �o�b�N�o�b�t�@�ɕ`��
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw();
	DebugInfoDraw();
	ScreenFlip();
	timeClass_.DeltaTimeEnd();
}

void SceneManager::Update()
{
	timeClass_.DeltaTimeStart();
	controller_->Update(timeClass_.GetDeltaTime<float>());
	scene_ = scene_->Update(std::move(scene_), timeClass_.GetDeltaTime<float>(), *controller_);
	timeClass_.GameTimeEnd();
}


ResourceMng& SceneManager::GetResourceMng(void)
{
	return *resourceMng_;
}

TimeClass& SceneManager::GetTimeClass(void)
{
	return timeClass_;
}

Random& SceneManager::GetRandomClass(void)
{
	return random_;
}

ThreadPool& SceneManager::GetThredPool(void)
{
	return *threadPool_;
}

Controller& SceneManager::GetController(void)
{
	return *controller_;
}

SceneManager::SceneManager() :
	isInit_{false}
{
	isInit_ = SysInit();
}

SceneManager::~SceneManager()
{
	InputConfig::Destroy();
	
	DxLib_End();
}


bool SceneManager::Init(void)
{
	bool rtn = Effekseer_Init(8000) != -1;

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// ���炩���ߍ쐬�X�N���[�����쐬���Ă���
	SharedRenderTargetHandle tmp;
	lpSceneMng.GetResourceMng().MakeRenderTarget(tmp, ScreenID::Loading, screenSize_<float>, true);
	lpSceneMng.GetResourceMng().MakeRenderTarget(tmp,ScreenID::Transition, screenSize_<float>, true);

	threadPool_->WaitAllTask();

	SetUseASyncLoadFlag(true);
	//scene_ = std::make_unique<Loading>(std::make_unique<GameScene>(), TransitionType::Fade, 1.0f);
	//scene_ = std::make_unique<FadeLoading>(std::make_unique<GameScene>(), 1.0f);
	scene_ = std::make_unique<FadeLoading>(std::make_unique<TitleScene>(), 1.0f);	DebugLog("�V�[���쐬�I��");
	timeClass_.DeltaTimeEnd();
	timeClass_.DeltaTimeStart();
	timeClass_.GameTimeEnd();
	return rtn;
}

bool SceneManager::SysInit(void)
{
	isEnd_ = false;
	// �������[���[�N�`�F�b�N�p�̂��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	DebugStart(timeClass_);
	SetWindowText(TEXT("�@�l"));
	SetWindowIconID(IDI_ICON1);
	SetGraphMode(screenSize_<int>.x, screenSize_<int>.y, 32);
	Set3DSoundOneMetre(64.0f);			// 3D��Ԃ�1���[�g�������̐ݒ�
	ChangeWindowMode(true);

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	bool rtn = DxLib_Init() != -1;
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	// dx�A�[�J�C�u�̃L�[���Z�b�g
	SetDXArchiveKeyString(L"data");

	// �}�E�X�J�[�\���̕\�����I�t��
	SetMouseDispFlag(false);

	// ���\�[�X�Ǘ��N���X���쐬
	resourceMng_ = std::make_unique<ResourceMng>();

	// �X���b�h�v�[�����쐬
	threadPool_ = std::make_unique<ThreadPool>(2);


	InputConfig::Create();

	if (InputConfig::GetInstance().GetNowType() != -1)
	{
		controller_ = std::make_unique<Pad>(InputConfig::GetInstance().GetNowType());
	}
	else
	{
		controller_ = std::make_unique<Keyboard>();
	}

	// �f�o�b�O�̃Z�b�g�A�b�v
	DebugSetUp();
	return rtn;
}
