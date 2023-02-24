#include<DxLib.h>
#include "GameScene.h"
#include "ResultScene.h"
#include "PauseScene.h"
#include "ClearScene.h"
#include "LoadScene.h"
#include "SceneMng.h"
#include "../Resource/Tmx/TmxObj.h"
#include "../Component/Collider/CircleCollider.h"
#include "Transition/ClearTrans.h"
#include "Transition/DeadTrans.h"
#include "../Object/UI/UIMng.h"
#include "../Object/Unit/Target.h"
#include "../Object/ObjManager.h"
#include "../common/Camera/CameraMode/CameraModeBase.h"
#include "../common/ImageMng.h"
#include "../common/GmkEvent.h"
#include "../common/ResultData.h"
#include "../common/SoundPross.h"
#include "../common/Debug.h"
#include "../common/DrawMng.h"
#include "../Object/Item/Clock.h"
#include "../common/ShaderMng.h"
#include "../common/ConstantBuffer.h"
#include "../Shader/ShaderDrawGraph.h"

GameScene::GameScene(int mapNum,bool isSmallMap) :
	isSmallMap_{isSmallMap}
{
	mapNum_ = mapNum;
	Init();
}

GameScene::~GameScene()
{
	DeleteGraph(viewID_);
	DeleteGraph(mapFloor_);
}

void GameScene::SoundPlay(void)
{
	//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::alarm);
	//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
	if (gEvent_->GetNowEventType() == EventType::Non)
	{
		SoundProcess::SoundPlay(SoundProcess::SOUNDNAME_BGM::stage, true, loopTime_);
	}
	else if (gEvent_->GetNowEventType() == EventType::Alert)
	{
		SoundProcess::SoundPlay(SoundProcess::SOUNDNAME_BGM::alarm, true, AlarmTime_);
	}

	for (const auto& c : objMng_->GetItemList())
	{
		if (c->GetObjID() == ObjID::Clock)
		{
			dynamic_cast<Clock&>(*c).RestartAlarm();
		}
	}

	loopTime_ = 0.0f;
	AlarmTime_ = 0.0f;
}

bool GameScene::Init(void)
{
	psID_ = ShaderID::GameFog;
	lpShaderMng.LoadpixelShader("Shader/GameFogPs.pso",psID_);
	cbuff_ = std::make_unique<ConstantBuffer<ScreenEffect>>();
	screenEffect_.radius = 120.0f;
	SetUseASyncLoadFlag(true);
	
	map_ = "tileset x1.png";

	// �Q�[���ɕK�v�ȕ�����������
	InitGame();

	// �J������������
	camera_.Init( *controller_, isSmallMap_, tmxObj_, objMng_->GetPlayer(), objMng_->GetTarget());
	
	// �X�N���[�������������
	InitScreen();

	SetUseASyncLoadFlag(false);

	viewPos_ = { 50,60 };

	return true;
}

bool GameScene::InitScreen(void)
{
	// �\���T�C�Y���擾
	const auto& viewSize = camera_.GetViewSize();

	// �X�N���[�����쐬
	viewID_ = MakeScreen(viewSize.x, viewSize.y, 255);

	// �}�b�v�S�̂̕\���}�X���ƃ`�b�v�T�C�Y���擾
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();

	// ���̕\���p�̃X�N���[�����쐬
	mapFloor_ = MakeScreen(static_cast<int>(worldArea.x * tileSize.x), static_cast<int>(worldArea.y * tileSize.y), true);
	
	// �`����Ǘ�����N���X���쐬
	drawMng_ = std::make_unique<DrawMng>(static_cast<float>(worldArea.y * tileSize.y));


	
	

	return true;
}

bool GameScene::InitGame(void)
{
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::stage, SoundProcess::GetBGMVolumeEntire(), true);

	// ���U���g�V�[���ɓn�����ʂ��Ǘ�����N���X���쐬
	resultData_ = std::make_shared<ResultData>();

	tmxObj_ = std::make_shared<TmxObj>();


	// �}�b�v�̓ǂݍ���
	bool rtn{ tmxObj_->LoadTMX("./Resource/tmx/map" + std::to_string(mapNum_) + ".tmx") };

	// �M�~�b�N�̃C�x���g�p�N���X���쐬
	gEvent_ = std::make_shared<GmkEvent>(resultData_);

	// UI�}�l�[�W���[���쐬
	uiMng_ = std::make_shared<UIMng>();

	// Obj�}�l�[�W���[���쐬
	objMng_ = std::make_unique<ObjManager>(
		"Resource/StageData/map" + std::to_string(mapNum_) +".data",
		resultData_,
		*(uiMng_->GetItemUI()),
		gEvent_, 
		tmxObj_, 
		screenSize_
		);
	
	lpImageMng.GetID("Resource/Image/fog/fog.png", "fog");
	lpImageMng.GetID("Resource/Image/fog/fogBG.png", "fogBG");

	// �M�~�b�N�C�x���g��obj�}�l�[�W���[���Z�b�g
	gEvent_->SetObjMng(objMng_);

	// �X�V�p�֐����J�n���̉��o�����̕����Z�b�g
	updateFunc_ = std::bind(&GameScene::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);

	drawFunc_ = std::bind(&GameScene::DrawGame, this, std::placeholders::_1);

	// �Q�[���J�n�O�Ȃ̂�false��
	startedFlag_ = false;

	zoomExt_ = 1.0;
	zoomPos_ = Math::Vector2(0, 0);
	loopTime_ = 0.0f;
	AlarmTime_ = 0.0f;
	SceneFlag_ = false;
	loopFlag_ = true;

	return rtn;
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
	return updateFunc_(delta, std::move(ownScene));
}

void GameScene::DrawOwnScreen(float delta)
{
	drawFunc_(delta);
}

uniqueScene GameScene::UpdateStart(float delta, uniqueScene ownScene)
{
	loopTime_ += delta;
	camera_.Update(delta);
	if (camera_.GetMode() == CamMode::Trace)
	{
		startedFlag_ = true;
		uiMng_->Update(delta, objMng_);
		if (uiMng_->FinStart())
		{
			updateFunc_ = std::bind(&GameScene::GameUpdate, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
	objMng_->UpdateStart(delta);
	DrawOwnScreen(delta);
	return ownScene;
}

uniqueScene GameScene::GameUpdate(float delta, uniqueScene ownScene)
{
	controller_->Update(delta);
	if (controller_->Pressed(InputID::btn1))
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::alarm);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
		for (const auto& c : objMng_->GetItemList())
		{
			if (c->GetObjID() == ObjID::Clock)
			{
				dynamic_cast<Clock&>(*c).StopAlarm();
			}
		}
		controller_->Update(delta);
		return std::make_unique<PauseScene>(std::move(ownScene),mapNum_, isSmallMap_,GetSceneID());
	}
	uiMng_->Update(delta, objMng_);
	DrawOwnScreen(delta);

	resultData_->UpdateClearTime(delta);
	if (!objMng_->Update(delta))
	{
		gEvent_->EndAlert();
		resultData_->SetClear(objMng_->IsClear());
		updateFunc_ = std::bind(&GameScene::UpdateGameEnd, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&GameScene::DrawGameEnd, this, std::placeholders::_1);
	}

	camera_.Update(delta);
	gEvent_->Update(delta);
	loopTime_ += delta;
	if (gEvent_->GetNowEventType() == EventType::Alert)
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
		AlarmTime_ += delta;
		loopTime_ = 0.0f;
		loopFlag_ = false;
	}
	if (loopTime_ >= 0.5f && !loopFlag_)
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::stage, SoundProcess::GetBGMVolumeEntire(), true);
		loopFlag_ = true;
	}

	return ownScene;
}

uniqueScene GameScene::UpdateGameEnd(float delta, uniqueScene ownScene)
{
	uiMng_->Update(delta, objMng_);

	//gEvent_->StartAlert();

	controller_->Update(delta);
	DrawOwnScreen(delta);
	objMng_->UpdateGameEnd(delta);

	camera_.Update(delta);
	gEvent_->Update(delta);
	if (objMng_->IsClear())
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::alarm);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		// �N���A���V�[���ڍs
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
		for (const auto& c : objMng_->GetItemList())
		{
			if (c->GetObjID() == ObjID::Clock)
			{
				dynamic_cast<Clock&>(*c).RestartAlarm();
			}
		}
		//return std::make_unique<ClearScene>(mapNum_);
		Relese();
		return std::make_unique<ClearTrans>(objMng_, *drawMng_, camera_, tmxObj_, gEvent_, std::move(ownScene), std::make_unique<ResultScene>(*resultData_, mapNum_, isSmallMap_, delta));
	}
	else
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::alarm);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		for (const auto& c : objMng_->GetItemList())
		{
			if (c->GetObjID() == ObjID::Clock)
			{
				dynamic_cast<Clock&>(*c).RestartAlarm();
			}
		}
		// �Q�[���I�[�o�[���V�[���ڍs
		Relese();
		return std::make_unique<DeadTrans>(objMng_, camera_.GetOffset(), *drawMng_, camera_, std::move(ownScene), std::make_unique<ResultScene>(*resultData_, mapNum_, isSmallMap_, delta));
	}
	return ownScene;
}

void GameScene::DrawGame(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	DrawMap();
	gEvent_->Draw();
	objMng_->Draw(camera_.GetOffset(), *drawMng_);
	drawMng_->Draw();

	
	// MyDrawGraph(0, 0, lpImageMng.GetID("fog")[0]);

	SetDrawScreen(screenID_);
	ClsDrawScreen();


	// DrawGraph(0, 0, lpImageMng.GetID("fogBG")[0], true);
	if (startedFlag_)
	{
		// ���W���Z�b�g(�v���C���[�̃I�t�Z�b�g�܂߂����W)
		screenEffect_.pos= objMng_->GetPlayer().lock()->GetPos() + camera_.GetOffset() + Math::Vector2{ 50.0f,60.0f };

		

		// �A���[�g���̒l�Z�b�g
		screenEffect_.redValue = gEvent_->Color();

		// �s�N�Z���V�F�[�_�Z�b�g
		SetUsePixelShader(lpShaderMng.Get(psID_));

		*cbuff_ = screenEffect_;
		cbuff_->Update();

		// �萔�o�b�t�@�Z�b�g
		SetShaderConstantBuffer(cbuff_->GetHandle(), DX_SHADERTYPE_PIXEL, 0);

		// fog�̃e�N�X�`���Z�b�g
		SetUseTextureToShader(1, lpImageMng.GetID("fog")[0]);

		// �`��
		MyDrawGraph(static_cast<int>(viewPos_.x), static_cast<int>(viewPos_.y), viewID_);
		uiMng_->Draw(delta);
	}
	else
	{
		DrawGraph(static_cast<int>(viewPos_.x), static_cast<int>(viewPos_.y), viewID_, true);
		DrawModiGraph(
			50, 60,
			50 + camera_.GetViewSize().x,60,
			50 + camera_.GetViewSize().x, 60 + camera_.GetViewSize().y,
			50, 60 + camera_.GetViewSize().y,
			lpImageMng.GetID("fog")[0], true);
		//DrawGraph(370, 100, lpImageMng.GetID("./Resource/Image/GameScene/clear.png")[0], true);
		DrawRotaGraph(lpSceneMng.GetScreenSize().x/2, 120, 1.0, 0.0, lpImageMng.GetID("./Resource/Image/GameScene/clear.png")[0], true);
	}
	gEvent_->MainDraw();
	camera_.Draw();
}

void GameScene::DrawGameEnd(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	SetFontSize(20);
	DrawMap();
	gEvent_->Draw();
	objMng_->Draw(camera_.GetOffset(), *drawMng_);
	drawMng_->Draw();
	camera_.Draw();
}

void GameScene::DrawMap(void)
{
	// �`��p�I�t�Z�b�g���擾
	const auto drawOffset = camera_.GetOffset();

	DrawGraph(static_cast<int>(drawOffset.x), static_cast<int>(drawOffset.y), mapFloor_, true);
	// �}�b�v�̕\��
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();

	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			//auto image = y * worldArea.x + x;
			// �O���t�B�b�Nid�̎擾
			int gid = tmxObj_->GetMapData("2Block", x, y) - tmxObj_->GetFirstGID();
			if (gid >= 0)
			{
				Math::Vector2 drawPos{ static_cast<float>(x) * tileSize.x , static_cast<float>(y) * tileSize.y };
				drawPos += drawOffset;
				drawMng_->Draw(drawPos,lpImageMng.GetID(map_)[gid]);
			}
		}
	}
}

void GameScene::Loaded(void)
{

	camera_.Loaded();
	SetDrawScreen(mapFloor_);
	ClsDrawScreen();
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();
	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			int gid = tmxObj_->GetMapData("1Floor", x, y) - tmxObj_->GetFirstGID();
			if (gid >= 0)
			{
				DrawGraph(x * tileSize.x, y * tileSize.x, lpImageMng.GetID(map_)[gid], true);
			}
		}
	}
	DrawOwnScreen(0.0f);
}

void GameScene::Relese(void)
{
	DeleteGraph(viewID_);
	DeleteGraph(mapFloor_);
	lpShaderMng.Relese(psID_);
}