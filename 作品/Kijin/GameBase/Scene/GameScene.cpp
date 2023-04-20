#include "GameScene.h"
#include <EffekseerForDXLib.h>
#include "../SceneManager.h"
#include "TitleScene.h"
#include "../Common/Camera.h"
#include "../Object/ObjectManager.h"
#include "../UI/UiManager.h"
#include "../Shader/PEManager.h"
#include "../Factory/StageFactory.h"
#include "PauseScene.h"
#include "../Common/ResourceMng.h"
#include "../Component/Transform/Transform.h"
#include "Transition/FadeLoading.h"
#include "../Common/SoundPross.h"
#include "ResultScene.h"
#include "../Common/SoundPross.h"
#include "../Component/Behavior/ThrusterBehavior.h"
#include "../Common/Input/InputConfig/InputConfig.h"
#include "../Component/Behavior/CameraBehavior.h"
#include "../Component/Behavior/StageBehavior.h"
#include "../Common/Debug.h"

GameScene::GameScene(StageID stageID) :
	BaseScene{ ScreenID::Game,SceneID::Game }, stageID_{stageID}
{
	SetMakeSceneFunc(std::bind(&GameScene::MakeResultFunc, this, std::placeholders::_1), SceneID::Result);
	SetMakeSceneFunc(std::bind(&GameScene::MakePauseFunc, this, std::placeholders::_1), SceneID::Pause);
	objMng_ = std::make_unique<ObjectManager>(10);
	peMng_ = std::make_unique<PEManager>();

	// �Q�[���V�[���p��BGM�̃��[�h
	lpSooundPross.Init(SceneID::Game);


	// �Q�[���V�[����BGM�̍Đ�
	lpSooundPross.PlayBackSound(SOUNDNAME_BGM::GameSceneStage1BGM, true, false);

	if (stageID_ == StageID::Tutorial)
	{
		// �`���[�g���A����
		uiMng_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true, true, false);
		objMng_->AddFactory(std::make_unique<StageFactory>(*objMng_));
		SetMakeSceneFunc(std::bind(&GameScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
	}
	else
	{
		// �ʏ펞�̃Q�[����
		objMng_->AddFactory(std::make_unique<StageFactory>("Resource/Other/Stage" + std::to_string(static_cast<int>(stageID)) + ".data", *objMng_));
		uiMng_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true,false, false);
		lpSceneMng.GetResourceMng().MakeRenderTarget(resultCapture_, ScreenID::ResultCapture, SceneManager::screenSize_<float>, true);
	}
	AddLoadedFunc(std::bind(&GameScene::Loaded, this, std::placeholders::_1));
	result_ = ResultAttribute::Max;

	// �Q�[���V�[���Ŏg���V�F�[�_�����炩���߃��[�h���Ă���
	useShaders_.resize(3);
	lpSceneMng.GetResourceMng().LoadVS(useShaders_[0], "Resource/resource/Shader/Vertex/Mesh.vso");
	lpSceneMng.GetResourceMng().LoadVS(useShaders_[1], "Resource/resource/Shader/Vertex/Mesh4.vso");
	lpSceneMng.GetResourceMng().LoadPS(useShaders_[2], "Resource/resource/Shader/Pixel/Tex.pso");

	SetLightDirection(VGet(1.0f, -1.0f, 1.0f));
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
	SetLightSpcColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.0f));

	int x, y;
	GetDrawScreenSize(&x, &y);
	// �V���h�E�}�b�v���쐬����ۂɍ����t�H�[�}�b�g�ݒ�͂���Ȃ��̂Őݒ��������
	// ���ɍ쐬����摜�̃t�H�[�}�b�g�̐ݒ������
	SetDrawValidFloatTypeGraphCreateFlag(true);
	// �`���l�����P�ɂ���
	SetCreateDrawValidGraphChannelNum(1);
	// �r�b�g����16�ɂ���
	SetCreateGraphColorBitDepth(16);
	// �V���h�E�}�b�v
	shadowMap_ = MakeScreen(x, y, false);

	// ���ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(false);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
	// ��ʊE�[�x
	depth_ = MakeScreen(x, y, false);

	// �I�t�X�N���[���̏�����
	offScreen_ = MakeScreen(x, y, false);
	// �X�J�C�h�[���̃X�N���[��
	skyScreen_ = MakeScreen(x, y, false);
	// �X�J�C�h�[���ƃX�e�[�W�̃X�N���[��
	subScreen_ = MakeScreen(x, y, false);

	// �V���h�E�}�b�v�p�J�������̏�����
	SetUseASyncLoadFlag(false);
	shadowBuff_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(shadowBuff_));
	lightMat.view = MGetIdent();
	lightMat.proj = MGetIdent();

	// ��ʊE�[�x�p�̃J�������̏�����
	depthbuffer_ = CreateShaderConstantBuffer(sizeof(DepthParam) * 4);
	depthMat_ = static_cast<DepthParam*>(GetBufferShaderConstantBuffer(depthbuffer_));
	depthMat.start = 0.0f;
	depthMat.end = 0.0f;
	depthMat.scope = 0.0f;
	SetUseASyncLoadFlag(true);

	SetAlwaysRunFlag(true);

#ifdef _DEBUG
	SetMakeSceneFunc(std::bind(&GameScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
#endif
}

GameScene::~GameScene()
{
	DeleteShaderConstantBuffer(shadowBuff_);
	DeleteShaderConstantBuffer(depthbuffer_);
	DeleteGraph(shadowMap_);
	DeleteGraph(depth_);
}

void GameScene::Capture(void)
{
	// ��xUi�ȊO��`�悷��
	SetupShadowMap();
	SetOffsetScreen();
	SetSubScreen();
	SetUpDepth();
	depthMat_[0] = depthMat;
	peMng_->SetBuffer(depthbuffer_);
	peMng_->Draw(offScreen_, *screenHandle_, depth_, skyScreen_, subScreen_);

	// �`�挋�ʂ��L���v�`������
	SetDrawScreen(*screenHandle_);
	GetDrawScreenGraph(0, 0, SceneManager::screenSize_<int>.x, SceneManager::screenSize_<int>.y, *resultCapture_);
}

void GameScene::SetUp(void)
{
	peMng_->SetFlag(PEID::Mono, lpConfigMng.GetPeConfig().at(PEID::Mono));
	peMng_->SetFlag(PEID::VolFog, lpConfigMng.GetPeConfig().at(PEID::VolFog));

	auto cam = objMng_->GetComponent<CameraBehavior>(objMng_->GetCameraID());
	if (cam.IsActive())
	{
		cam->SetSpeed(lpConfigMng.GetCameraSpeed());
	}
}

BaseScene::SceneUptr GameScene::MakeResultFunc(SceneUptr own)
{
	SetAlwaysRunFlag(false);
	Capture();

	// ���U���g�ł̓��[�v����Ă���T�E���h���~�߂�i�O�̂��߂ɃA�t�^�[�o�[�i�[���~�߂�)
	lpSooundPross.SoundStop(SOUNDNAME_BGM::GameSceneStage1BGM);
	lpSooundPross.SoundStop(SOUNDNAME_SE::playerMove);
	lpSooundPross.SoundStop(SOUNDNAME_SE::playerAB);

	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<ResultScene>(stageID_ ,result_, resultCapture_), 1.0f);
}

BaseScene::SceneUptr GameScene::MakePauseFunc(SceneUptr own)
{
	SetAlwaysRunFlag(false);
	return std::make_unique<PauseScene>(std::move(own));
}

BaseScene::SceneUptr GameScene::MakeSelectFunc(SceneUptr own)
{
	SetAlwaysRunFlag(false);
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<TitleScene>(), 0.5f);
}

void GameScene::Update(float delta, Controller& controller)
{
	if (controller.PressdCancel() || !GetWindowActiveFlag())
	{
		ChangeSceneID(SceneID::Pause);
		return;
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeSceneID(SceneID::Select);
		return;
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		SetResult(ResultAttribute::Clear);
		ChangeSceneID(SceneID::Result);
		return;
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		/*auto id = objMng_->GetFactory(FactoryID::HorizontalEffect).Create(ObjectID{}, {0.0f, 100.0f, 0.0f});
		objMng_->Begin(id);*/
	}
#endif
	
	auto player = (objMng_->GetComponent<Transform>(objMng_->GetPlayerID()));
	objMng_->Update(delta, controller, *this);
	uiMng_->Update(delta, *this, *objMng_, controller);
	peMng_->Update(delta);

	// �G�t�F�N�V�A�̍X�V
	UpdateEffekseer3D();
}

void GameScene::DrawScene(void)
{
	// �V���h�E�}�b�v�쐬
	SetupShadowMap();
	// �I�t�X�N���[���̍쐬
	SetOffsetScreen();
	// ���C���ȊO�̃X�N���[���̍쐬
	SetSubScreen();

	// ��ʊE�[�x�p�̐[�x�e�N�X�`���̍쐬
	SetUpDepth();

	depthMat_[0] = depthMat;
	peMng_->SetBuffer(depthbuffer_);
	// �|�X�g�G�t�F�N�g���ʏ�`��
	peMng_->Draw(offScreen_, *screenHandle_,depth_,skyScreen_,subScreen_);

	// UI�̕`��
	uiMng_->Draw(*screenHandle_);
}

void GameScene::SetupShadowMap(void)
{
	// �`�����e�p�[�x�L�^�摜�ɕύX����
	SetDrawScreen(shadowMap_);
	// �e�p�[�x�L�^�摜����x�^�����ɂ���
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);

	// �V���h�E�}�b�v�p�ɃJ�������Z�b�g
	auto player = objMng_->GetComponent<Transform>(objMng_->GetPlayerID());
	if (player.IsActive())
	{
		auto& pPos = player->GetPos();
		Vector3 pTar = Vector3(pPos.x + camTar.x, camTar.y, pPos.z - camTar.z);
		camera_->SetUpShadow(offsetOrtho, offsetNear, offsetFar, pTar);
	}

	MV1SetUseOrigShader(true);
	objMng_->SetupDepthTex(*shadowPs_, -1);
	MV1SetUseOrigShader(false);

	// �V���h�E�}�b�v�쐬�Ɏg�����J���������擾
	lightMat.view = GetCameraViewMatrix();
	lightMat.proj = GetCameraProjectionMatrix();
	// �`��p�ɐ؂�ւ�
	SetDrawScreen(offScreen_);
	ClsDrawScreen();
	// �������擾�����J��������hlsl���ɓn��
	lightMat_[0] = lightMat;
}

void GameScene::SetUpDepth(void)
{
	// �`�����e�p�[�x�L�^�摜�ɕύX
	SetDrawScreen(depth_);
	// �e�p�[�x�L�^�摜����x�^�����ɂ���
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);
	camera_->SetUpScreen();

	// ��ʊE�[�x�J�n�ʒu�̌v�Z
	depthMat.start = dofFocus - dofFocusSize / 2.0f - dofInterpSize;
	// ��ʊE�[�x�I���ʒu���v�Z
	depthMat.end = dofFocus + dofFocusSize / 2.0f + dofInterpSize;
	// ��ʊE�[�x�͈̔͂̋t�����v�Z
	depthMat.scope = 1.0f / (depthMat.end - depthMat.start);
	// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
	dofTotalSize_ = dofInterpSize * 2.0f + dofFocusSize;

	// �擾�����f�[�^��hlsl���ɓn��
	depthMat_[0] = depthMat;
	MV1SetUseOrigShader(true);
	objMng_->SetupDepthTex(*depthPS_, depthbuffer_);
	MV1SetUseOrigShader(false);

	// �`��p�ɐ؂�ւ�
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	// ��ʊE�[�x�J�n�ʒu�̌v�Z
	depthMat.start = dofInterpSize / dofTotalSize_;
	// ��ʊE�[�x�I���ʒu�̌v�Z
	depthMat.end = (dofInterpSize + dofFocusSize) / dofTotalSize_;

}

void GameScene::SetOffsetScreen(void)
{
	camera_->SetUpScreen();	

	// �`��
	objMng_->ShadowDraw(shadowMap_,shadowBuff_);
	MV1SetUseOrigShader(false);

	// �G�t�F�N�V�A�̕`��
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();

	// 3D�n�̃f�o�b�O�`��
	DebugDraw3DScreen();

	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
}

void GameScene::SetSubScreen(void)
{
	// �X�J�C�h�[���݂̂̃X�N���[��
	SetDrawScreen(skyScreen_);
	ClsDrawScreen();
	camera_->SetUpScreen();
	auto [result, id] = objMng_->Find(ObjectAttribute::Sky);
	objMng_->GetComponent<Render>(id)->Draw();
	
	// �X�J�C�h�[���ƃX�e�[�W�݂̂̃X�N���[��
	SetDrawScreen(subScreen_);
	ClsDrawScreen();
	camera_->SetUpScreen();
	auto [result1, id1] = objMng_->Find(ObjectAttribute::Stage);
	objMng_->GetComponent<Render>(id)->Draw();
	objMng_->GetComponent<Render>(id1)->Draw();
}

bool GameScene::IsLoaded(void)
{
	return BaseScene::IsLoaded() && objMng_->IsLoaded() && uiMng_->IsLoaded();
}

void GameScene::Loaded(Controller& controller)
{
	// ���[�h�������ɌĂ΂��
	lpSceneMng.GetResourceMng().Loaded();
	objMng_->Begin(); 
	objMng_->Update(0.0f, controller, *this);
	uiMng_->Begin(*this);

	
	lpSceneMng.GetResourceMng().LoadPS(shadowPs_, "Resource/resource/Shader/ShadowMap/ShadowMap.pso");
	lpSceneMng.GetResourceMng().LoadPS(depthPS_, "Resource/resource/Shader/PostEffect/Dof/depth.pso");
	SetUp();
	UpdateEffekseer3D();
}

void GameScene::Back(void)
{
	SetAlwaysRunFlag(true);
}
