#include<DxLib.h>
#include "GameScene.h"
#include "ResultScene.h"
#include "PauseScene.h"
#include "../common/ImageMng.h"
#include "../common/GmkEvent.h"
#include "../Object/ObjManager.h"
#include "../common/Debug.h"
#include "../Component/Collider/CircleCollider.h"
#include "../common/DrawMng.h"
#include "../Resource/Tmx/TmxObj.h"
#include "../common/Camera/CameraMode/CameraModeBase.h"
#include "../Object/UI/UIMng.h"
#include "Transition/ClearTrans.h"
#include "SceneMng.h"
#include "../Object/Unit/Target.h"
#include "Transition/DeadTrans.h"
#include "../common/ResultData.h"
#include "ClearScene.h"
#include "../common/SoundPross.h"
#include "../common/SoundPross.h"
#include "../Input/InputConfig.h"

ClearScene::ClearScene(int mapNum, bool isSmallMap)
{
	mapNum_ = mapNum;
	Init();
}

ClearScene::~ClearScene()
{
	DeleteGraph(viewID_);
	DeleteGraph(mapFloor_);
}

bool ClearScene::Init(void)
{
	SetUseASyncLoadFlag(true);

	map_ = "tileset x1.png";

	// �Q�[���ɕK�v�ȕ�����������
	InitGame();

	// �J������������
	camera_.Init(*controller_, false, tmxObj_, objMng_->GetPlayer(), objMng_->GetTarget());

	// �X�N���[�������������
	InitScreen();

	SetUseASyncLoadFlag(false);
	return true;
}

bool ClearScene::InitScreen(void)
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
	return false;
}

bool ClearScene::InitGame(void)
{
	// ���U���g�V�[���ɓn�����ʂ��Ǘ�����N���X���쐬
	resultData_ = std::make_shared<ResultData>();

	tmxObj_ = std::make_shared<TmxObj>();

	//mapNum_ = 9;
	// �}�b�v�̓ǂݍ���
	bool rtn{ tmxObj_->LoadTMX("./Resource/tmx/map" + std::to_string(mapNum_) + ".tmx") };

	// �M�~�b�N�̃C�x���g�p�N���X���쐬
	gEvent_ = std::make_shared<GmkEvent>(resultData_);

	// UI�}�l�[�W���[���쐬
	uiMng_ = std::make_shared<UIMng>();

	// Obj�}�l�[�W���[���쐬
	objMng_ = std::make_unique<ObjManager>(
		"Resource/map" + std::to_string(mapNum_) + ".data",
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
	updateFunc_ = std::bind(&ClearScene::GameUpdate, this, std::placeholders::_1, std::placeholders::_2);

	drawFunc_ = std::bind(&ClearScene::DrawGame, this, std::placeholders::_1);

	// �Q�[���J�n�O�Ȃ̂�false��
	//startedFlag_ = false;

	lpImageMng.GetID("./Resource/Input/Keydata.png", "Keydata", Vector2I(75, 25), Vector2I(10, 6));
	lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));

	zoomExt_ = 1.0;
	zoomPos_ = Math::Vector2(0, 0);

	SceneFlag_ = false;

	return rtn;
}

uniqueScene ClearScene::Update(float delta, uniqueScene ownScene)
{
	return updateFunc_(delta, std::move(ownScene));
}

void ClearScene::DrawOwnScreen(float delta)
{
	drawFunc_(delta);
}

//uniqueScene ClearScene::UpdateStart(float delta, uniqueScene ownScene)
//{
//	camera_.Update(delta);
//	if (camera_.GetMode() == CamMode::Trace)
//	{
//		startedFlag_ = true;
//		updateFunc_ = std::bind(&ClearScene::GameUpdate, this, std::placeholders::_1, std::placeholders::_2);
//	}
//	objMng_->UpdateStart(delta);
//	DrawOwnScreen(delta);
//	return ownScene;
//}

uniqueScene ClearScene::GameUpdate(float delta, uniqueScene ownScene)
{
	controller_->Update(delta);
	if (controller_->Pressed(InputID::btn1))
	{
		controller_->Update(delta);
		return std::make_unique<PauseScene>(std::move(ownScene), mapNum_, false, GetSceneID());
	}
	uiMng_->Update(delta, objMng_);
	DrawOwnScreen(delta);

	resultData_->UpdateClearTime(delta);
	if (!objMng_->Update(delta))
	{
		gEvent_->EndAlert();
		resultData_->SetClear(objMng_->IsClear());
		updateFunc_ = std::bind(&ClearScene::UpdateGameEnd, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&ClearScene::DrawGameEnd, this, std::placeholders::_1);
	}

	camera_.Update(delta);
	gEvent_->Update(delta);
	return ownScene;
}

uniqueScene ClearScene::UpdateGameEnd(float delta, uniqueScene ownScene)
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
		// �N���A���V�[���ڍs
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
	}
	else
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
		// �Q�[���I�[�o�[���V�[���ڍs
		//return std::make_unique<DeadTrans>(objMng_, camera_.GetOffset(), *drawMng_, std::move(ownScene), std::make_unique<ResultScene>(*resultData_, mapNum_));
	}
	return ownScene;
}

void ClearScene::DrawGame(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	SetFontSize(20);
	DrawMap();
	gEvent_->Draw();
	objMng_->Draw(camera_.GetOffset(), *drawMng_);
	drawMng_->Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);

	DrawGraph(0, 0, lpImageMng.GetID("fog")[0], true);
	// DrawGraph(0, 0, lpImageMng.GetID("fogBG")[0], true);

	uiMng_->Draw(delta);

	gEvent_->MainDraw();
	camera_.Draw();
}

void ClearScene::DrawGameEnd(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	SetFontSize(20);
	DrawMap();
	gEvent_->Draw();
	objMng_->Draw(camera_.GetOffset(), *drawMng_);
	drawMng_->Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
	uiMng_->Draw(delta);
	camera_.Draw();
}

void ClearScene::DrawMap(void)
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
			// �O���t�B�b�Nid�̎擾
			int gid = tmxObj_->GetMapData("2Block", x, y) - tmxObj_->GetFirstGID();
			if (gid >= 0)
			{
				Math::Vector2 drawPos{ static_cast<float>(x) * tileSize.x , static_cast<float>(y) * tileSize.y };
				drawPos += drawOffset;
				//drawMng_->Draw(drawPos, (tileSize / 2), lpImageMng.GetID(map_)[gid]);
				drawMng_->Draw(drawPos, lpImageMng.GetID(map_)[gid]);
			}
		}
	}

}

void ClearScene::Loaded(void)
{
	SetDrawScreen(mapFloor_);
	ClsDrawScreen();
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();
	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			auto t = tmxObj_->GetMapData("1Floor", x, y) - tmxObj_->GetFirstGID();
			DrawGraph(x * tileSize.x, y * tileSize.x, lpImageMng.GetID(map_)[t], true);
		}
	}
	DrawOwnScreen(0.0f);
}