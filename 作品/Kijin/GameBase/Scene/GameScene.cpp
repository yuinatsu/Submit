#include "GameScene.h"
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "TitleScene.h"
#include "../Common/Camera.h"
#include "../Object/ObjectManager.h"
#include "../UI/UiManager.h"
#include "../Factory/StageFactory.h"
#include "PauseScene.h"
#include "../Common/ResourceMng.h"
#include "../Component/Transform/Transform.h"
#include "Transition/Loading.h"
#include "../Common/Debug.h"

#include "../Common/SoundPross.h"
#include "Transition/Fade.h"
#include "Transition/TransitionType.h"
#include "ResultScene.h"
#include "../Common/SoundPross.h"

GameScene::GameScene() :
	BaseScene{ScreenID::Game,SceneID::Game}
{
	SetMakeSceneFunc(std::bind(&GameScene::MakeResultFunc,this, std::placeholders::_1),SceneID::Result);
	objMng_ = std::make_unique<ObjectManager>(10);
	objMng_->AddFactory(std::make_unique<StageFactory>("Resource/Other/Stage.data", * objMng_));

	uiMng_ = std::make_unique<UiManager>(SceneID::Game);
	result_ = ResultAttribute::Max;

	// �Q�[���V�[���Ŏg���V�F�[�_�����炩���߃��[�h���Ă���
	useShaders_.resize(3);
	lpResourceMng.LoadVS(useShaders_[0], "Resource/resource/Shader/Vertex/Mesh.vso");
	lpResourceMng.LoadVS(useShaders_[1], "Resource/resource/Shader/Vertex/Mesh4.vso");
	lpResourceMng.LoadPS(useShaders_[2], "Resource/resource/Shader/Pixel/Tex.pso");
	
	SetFrontScene(std::make_unique<PauseScene>());

	AddLoadedFunc(std::bind(&GameScene::Loaded, this, std::placeholders::_1));

	SetLightDirection(VGet(1.f, -1.f, 1.f));
	SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightSpcColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.f));
	PostTex_ = MakeScreen(1280, 720, false);

	// �쐬����摜�̃t�H�[�}�b�g��1�`���l���A16�r�b�g��s��
	SetDrawValidFloatTypeGraphCreateFlag(true);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);
	shadowMap_ = MakeScreen(1280, 720, false);
	// ���ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(false);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
	
	// �J�������̏�����
	cbuffer_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	auto p = GetBufferShaderConstantBuffer(cbuffer_);
	lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(cbuffer_));
	lightM_.view = MGetIdent();
	lightM_.proj = MGetIdent();

	// �Q�[���V�[���p��BGM�̃��[�h
	SoundProcess::Init(SceneID::Game);
}

BaseScene::SceneUptr GameScene::MakeResultFunc(SceneUptr own)
{
	return std::make_unique<Fade>(std::move(own), std::make_unique<Loading>(std::make_unique<ResultScene>(result_), TransitionType::Fade, 1.0f, false), 1.0f);
}

void GameScene::Update(float delta, Controller& controller)
{
	if (controller.Pressed(InputID::btn1))
	{
		ChangeSceneID(SceneID::Pause);
		return;
	}
	
	
	auto player = (objMng_->GetComponent<Transform>(objMng_->GetPlayerID()));
	objMng_->Update(delta, controller, *this);
	// �Q�[���I������
	if (!player.IsActive())
	{
		// �Q�[���I�[�o�[
		result_ = ResultAttribute::GameOver;
		ChangeSceneID(SceneID::Result);
		SoundProcess::Release();
	}
	uiMng_->Update(delta, *this, *objMng_, controller);
	camera_->SetPos(player->GetPos());
	camera_->Update(controller);

	// �G�t�F�N�V�A�̍X�V
	//UpdateEffekseer3D();
}

void GameScene::DrawScene(void)
{
	//// �V���h�E�}�b�v�쐬
	//SetupShadowMap();
	//ClsDrawScreen();
	//SetupBokeTex();
	//ClsDrawScreen();
	//camera_->SetUpScreen();
	//// �`��
	//objMng_->ShadowDraw(shadowMap_,cbuffer_);
	////objMng_->Draw();
	//// �G�t�F�N�V�A�̕`��
	////DrawEffekseer3D();
	//// 3D�n�̃f�o�b�O�`��
	//DebugDraw3DScreen();
	//// UI�̕`��
	//uiMng_->Draw();
	////DrawRotaGraph(133, 83, 0.25, 0.0, shadowMap_, true);
	//DrawRotaGraph(133, 83, 0.25, 0.0, bokeTex_, true);
	//// �|�X�g�G�t�F�N�g
	//DrawFormatString(0, 0, 0xffffff, TEXT("%dID�̃V�[���ł�"), static_cast<unsigned int>(scID_));
	
	// �V���h�E�}�b�v�쐬
	SetupShadowMap();
	SetupPostEffect();
	ClsDrawScreen();
	
	// �|�X�g�G�t�F�N�g
	//SetPostEffect(true, 0, 0, Tex_, PostPS_);
	SetPostEffect(false, 0, 0, PostTex_, PostPS_);

	// UI�̕`��
	uiMng_->Draw();

	DrawRotaGraph(133, 83, 0.25, 0.0, shadowMap_, true);
	//DrawRotaGraph(133, 83, 0.25, 0.0, bokeTex_, true);


	DrawFormatString(0, 0, 0xffffff, TEXT("%dID�̃V�[���ł�"), static_cast<unsigned int>(scID_));
}

void GameScene::SetupShadowMap(void)
{
	// �`�����e�p�[�x�L�^�摜�ɕύX����
	SetDrawScreen(shadowMap_);
	// �e�p�[�x�L�^�摜����x�^�����ɂ���
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);
	// �J�����̕`��͈͂ƌ�����ʒu��ݒ�
	SetupCamera_Ortho(offsetOrtho);
	SetCameraNearFar(offsetNear, offsetFar);
	// �J�����̌��������C�g�̌����ɍ��킹��
	auto lightDir = GetLightDirection();
	auto lightPos = VAdd(VGet(camTar.x, camTar.y, camTar.z), VScale(lightDir, -5000));
	SetCameraPositionAndTarget_UpVecY(lightPos, VGet(camTar.x, camTar.y, camTar.z));
	MV1SetUseOrigShader(true);
	objMng_->SetupShadowMap();
	MV1SetUseOrigShader(false);

	lightM_.view = GetCameraViewMatrix();
	lightM_.proj = GetCameraProjectionMatrix();
	// �`��p�ɐ؂�ւ�
	SetDrawScreen(PostTex_);
	ClsDrawScreen();
	lightMat_[0] = lightM_;
}

void GameScene::SetupPostEffect(void)
{
	camera_->SetUpScreen();

	// �`��

	objMng_->ShadowDraw(shadowMap_, cbuffer_);
	MV1SetUseOrigShader(false);


	//objMng_->Draw();

	// �G�t�F�N�V�A�̕`��
	//DrawEffekseer3D();

	// 3D�n�̃f�o�b�O�`��
	DebugDraw3DScreen();


	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
}


void GameScene::SetPostEffect(bool flag, int x, int y, int img, int Postps)
{
	int width, height;
	GetGraphSize(img, &width, &height);
	std::array <VERTEX2DSHADER, 4> verts;

	if (flag)
	{
		for (auto& v : verts)
		{
			v.rhw = 1.0;
			v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff); // �f�B�t���[�Y
			v.spc = GetColorU8(255, 255, 255, 255);		// �X�y�L����
			v.su = 0.0f;
			v.sv = 0.0f;
			v.pos.z = 0.0f;
		}
		// ����
		verts[0].pos.x = x;
		verts[0].pos.y = y;
		verts[0].u = 0.0f;
		verts[0].v = 0.0f;
		// �E��
		verts[1].pos.x = x + width;
		verts[1].pos.y = y;
		verts[1].u = 1.0f;
		verts[1].v = 0.0f;
		// ����
		verts[2].pos.x = x;
		verts[2].pos.y = y + height;
		verts[2].u = 0.0f;
		verts[2].v = 1.0f;
		// �E��
		verts[3].pos.x = x + width;
		verts[3].pos.y = y + height;
		verts[3].u = 1.0f;
		verts[3].v = 1.0f;
		SetUsePixelShader(Postps);

		SetUseTextureToShader(0, img);
		DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
		MV1SetUseOrigShader(false);
	}
	else if (!flag)
	{
		DrawGraph(0, 0, img, true);
	}
	SetUseTextureToShader(0, -1);
}

bool GameScene::IsLoaded(void)
{
	if (BaseScene::IsLoaded() && objMng_->IsLoaded())
	{
		return true;
	}
	return false;
}

void GameScene::Loaded(Controller& controller)
{
	// ���[�h�������ɌĂ΂��
	lpResourceMng.Loaded();
	objMng_->Begin(); 
	objMng_->Update(0.0f, controller, *this);
	auto player = (objMng_->GetComponent<Transform>(objMng_->GetPlayerID()));
	camera_->SetPos(player->GetPos());
	camera_->Update(controller);
	uiMng_->Begin();

	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::GameSceneBGM, SoundProcess::GetVolume(), true);	// �Q�[���V�[����BGM
	PostPS_ = LoadPixelShader(L"Resource/resource/Shader/PostEffect/mono.pso");
}
