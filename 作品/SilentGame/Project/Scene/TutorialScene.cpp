#include<DxLib.h>
#include "TutorialScene.h"
#include "ResultScene.h"
#include "PauseScene.h"
#include "../common/ImageMng.h"
#include "../common/GmkEvent.h"
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
#include "LoadScene.h"
#include "SelectScene.h"
#include "Transition/CircleScene.h"
#include "../Input/InputConfig.h"

#include "../Shader/ShaderDrawGraph.h"

constexpr char key[]{ "./Resource/Input/Keydata.png" };
constexpr char pad[]{ "./Resource/Input/Paddata.png" };
constexpr char padPS[]{ "./Resource/Input/PaddataPS.png" };


TutorialScene::TutorialScene()
{
}

TutorialScene::~TutorialScene()
{
	DeleteGraph(viewID_);
	DeleteGraph(mapFloor_);
	DeleteShader(psH_);
	DeleteShaderConstantBuffer(cbuffH_);
}

void TutorialScene::SoundPlay(void)
{
	SoundProcess::SoundPlay(SoundProcess::SOUNDNAME_BGM::stage, true, loopTime_);
	loopTime_ = 0.0f;
}

bool TutorialScene::Init(void)
{
	psH_ = LoadPixelShader("Shader/GameFogPs.pso");
	cbuffH_ = CreateShaderConstantBuffer(static_cast<int>((sizeof(ScreenEffect) + 16) & ~16));
	buff_ = reinterpret_cast<ScreenEffect*>(GetBufferShaderConstantBuffer(cbuffH_));
	SetUseASyncLoadFlag(true);

	map_ = "tileset x1.png";

	// �Q�[���ɕK�v�ȕ�����������
	InitGame();

	// �J������������
	camera_.Init(false, tmxObj_, objMng_->GetPlayer());

	// �X�N���[�������������
	InitScreen();

	SetUseASyncLoadFlag(false);
	return true;
}

bool TutorialScene::InitScreen(void)
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

bool TutorialScene::InitGame(void)
{
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::stage, SoundProcess::GetBGMVolumeEntire(), true);

	// ���U���g�V�[���ɓn�����ʂ��Ǘ�����N���X���쐬
	resultData_ = std::make_shared<ResultData>();

	tmxObj_ = std::make_shared<TmxObj>();

	// �}�b�v�̓ǂݍ���
	bool rtn{ tmxObj_->LoadTMX("./Resource/tmx/Tmap" + std::to_string(mapNum_) + ".tmx") };

	// �M�~�b�N�̃C�x���g�p�N���X���쐬
	gEvent_ = std::make_shared<GmkEvent>(resultData_);

	// UI�}�l�[�W���[���쐬
	uiMng_ = std::make_shared<UIMng>();

	// Obj�}�l�[�W���[���쐬
	objMng_ = std::make_unique<ObjManager>(
		"Resource/StageData//Tutorial"+std::to_string(mapNum_) + ".data",
		resultData_,
		*(uiMng_->GetItemUI()),
		gEvent_,
		tmxObj_,
		screenSize_
		);

	lpImageMng.GetID("Resource/Image/fog/fog.png", "fog");
	lpImageMng.GetID("Resource/Image/fog/fogBG.png", "fogBG");

	keydata_ = lpImageMng.GetID(key, "Keydata", Vector2I(75, 75), Vector2I(10, 6));
	auto padType = GetJoypadType(DX_INPUT_PAD1);
	paddata_ = lpImageMng.GetID(pad, "Xbox", Vector2I(75, 25), Vector2I(10, 1));
	if (padType == 3 || padType == 4)
	{
		paddata_ = lpImageMng.GetID(padPS, "PS", Vector2I(90, 25), Vector2I(10, 2));
	}

	boxItem_ = objMng_->GetBoxItem();
	// �M�~�b�N�C�x���g��obj�}�l�[�W���[���Z�b�g
	gEvent_->SetObjMng(objMng_);

	// �X�V�p�֐����J�n���̉��o�����̕����Z�b�g
	updateFunc_ = std::bind(&TutorialScene::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);

	drawFunc_ = std::bind(&TutorialScene::DrawGame, this, std::placeholders::_1);

	// �Q�[���J�n�O�Ȃ̂�false��
	startedFlag_ = false;

	zoomExt_ = 1.0;
	TutorialType_ = 0;
	count_ = 0.0f;
	loopTime_ = 0.0f;
	zoomPos_ = Math::Vector2(0, 0);
	sumFlag_ = false;
	moveFlag_ = false;
	hitFlag_ = false;
	SceneFlag_ = false;
	updownFalg_ = false;
	startedFlag_ = false;
	speed_ = 0.0f;
	return rtn;
}

uniqueScene TutorialScene::Update(float delta, uniqueScene ownScene)
{
	return updateFunc_(delta, std::move(ownScene));
}

void TutorialScene::DrawOwnScreen(float delta)
{
	drawFunc_(delta);
}

uniqueScene TutorialScene::UpdateStart(float delta, uniqueScene ownScene)
{
	loopTime_ += delta;
	camera_.Update(delta);
	DrawOwnScreen(delta);
	updateFunc_ = std::bind(&TutorialScene::GameUpdate, this, std::placeholders::_1, std::placeholders::_2);
	startedFlag_ = true;
	return ownScene;
}

uniqueScene TutorialScene::GameUpdate(float delta, uniqueScene ownScene)
{
	loopTime_ += delta;
	controller_->Update(delta);
	if (controller_->Pressed(InputID::btn1))
	{
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::stage);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		controller_->Update(delta);
		return std::make_unique<PauseScene>(std::move(ownScene), mapNum_, false,GetSceneID());
	}
	uiMng_->Update(delta, objMng_);
	DrawOwnScreen(delta);

	resultData_->UpdateClearTime(delta);
	// �����̕����͌�ŕ����邱��
	if (controller_->Pressed(InputID::Attack) && !sumFlag_ && count_ > 0.1f)
	{
		sumFlag_ = true;
		count_ = 0.0f;
	}
	if (sumFlag_)
	{
		if (!UpdateTutorial(delta))
		{
			gEvent_->EndAlert();
			resultData_->SetClear(objMng_->IsClear());
			updateFunc_ = std::bind(&TutorialScene::UpdateGameEnd, this, std::placeholders::_1, std::placeholders::_2);
			drawFunc_ = std::bind(&TutorialScene::DrawGameEnd, this, std::placeholders::_1);
		}
	}
	camera_.Update(delta);
	gEvent_->Update(delta);
	count_ += delta;
	return ownScene;
}

uniqueScene TutorialScene::UpdateGameEnd(float delta, uniqueScene ownScene)
{
	uiMng_->Update(delta, objMng_);

	controller_->Update(delta);
	DrawOwnScreen(delta);
	objMng_->UpdateGameEnd(delta);

	camera_.Update(delta);
	gEvent_->Update(delta);
	if (objMng_->IsClear())
	{
		// �N���A���V�[���ڍs
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		return std::make_unique<CircleScene>(
			2.0,
			std::move(ownScene),
			std::make_unique<SelectScene>()
			);

	}
	return ownScene;
}

void TutorialScene::UpdateSwitching(float count, bool moveFlag, bool hitFlag)
{
	if (controller_->Pressed(InputID::Attack) && count_ > count)
	{
		count_ = 0.0f;
		TutorialType_++;
		moveFlag_ = moveFlag;
		hitFlag_ = hitFlag;
	}
}

void TutorialScene::DrawGame(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	DrawMap();
	gEvent_->Draw();
	DrawTutorial(delta);
	drawMng_->Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
	if (startedFlag_)
	{
		// ���W���Z�b�g(�v���C���[�̃I�t�Z�b�g�܂߂����W)
		buff_->pos = objMng_->GetPlayer().lock()->GetPos() + camera_.GetOffset() + Math::Vector2{ 50.0f,60.0f };

		// ���a�Z�b�g
		buff_->radius = 120.0f;

		// �A���[�g���̒l�Z�b�g
		buff_->redValue = gEvent_->Color();

		// �s�N�Z���V�F�[�_�Z�b�g
		SetUsePixelShader(psH_);

		// �萔�o�b�t�@�X�V
		UpdateShaderConstantBuffer(cbuffH_);

		// �萔�o�b�t�@�Z�b�g
		SetShaderConstantBuffer(cbuffH_, DX_SHADERTYPE_PIXEL, 0);

		// fog�̃e�N�X�`���Z�b�g
		SetUseTextureToShader(1, lpImageMng.GetID("fog")[0]);

		// �`��
		MyDrawGraph(50, 60, viewID_);
		uiMng_->Draw(delta);
	}
	else
	{
		DrawGraph(50, 60, viewID_, true);
		DrawModiGraph(
			50, 60,
			50 + camera_.GetViewSize().x, 60,
			50 + camera_.GetViewSize().x, 60 + camera_.GetViewSize().y,
			50, 60 + camera_.GetViewSize().y,
			lpImageMng.GetID("fog")[0], true);
		DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, 120, 1.0, 0.0, lpImageMng.GetID("./Resource/Image/GameScene/clear.png")[0], true);
	}
	DrawSummary(delta);
	DrawDescription(delta);
	gEvent_->MainDraw();
	camera_.Draw();

}

void TutorialScene::DrawGameEnd(float delta)
{
	// �������g�̃X�N���[���ɑ΂���Draw
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	DrawMap();
	gEvent_->Draw();
	objMng_->Draw(camera_.GetOffset(), *drawMng_);
	drawMng_->Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
	if (startedFlag_)
	{
		uiMng_->Draw(delta);
	}
	camera_.Draw();
}

void TutorialScene::DrawMap(void)
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
				drawMng_->Draw(drawPos, lpImageMng.GetID(map_)[gid]);
			}
		}
	}
}

void TutorialScene::Loaded(void)
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
			auto t = tmxObj_->GetMapData("1Floor", x, y) - tmxObj_->GetFirstGID();
			DrawGraph(x * tileSize.x, y * tileSize.x, lpImageMng.GetID(map_)[t], true);
		}
	}
	DrawOwnScreen(0.0f);
}

