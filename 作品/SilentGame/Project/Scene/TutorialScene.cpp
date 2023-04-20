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

	// ゲームに必要な部分を初期化
	InitGame();

	// カメラを初期化
	camera_.Init(false, tmxObj_, objMng_->GetPlayer());

	// スクリーン周りを初期化
	InitScreen();

	SetUseASyncLoadFlag(false);
	return true;
}

bool TutorialScene::InitScreen(void)
{
	// 表示サイズを取得
	const auto& viewSize = camera_.GetViewSize();

	// スクリーンを作成
	viewID_ = MakeScreen(viewSize.x, viewSize.y, 255);

	// マップ全体の表示マス数とチップサイズを取得
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();

	// 床の表示用のスクリーンを作成
	mapFloor_ = MakeScreen(static_cast<int>(worldArea.x * tileSize.x), static_cast<int>(worldArea.y * tileSize.y), true);

	// 描画を管理するクラスを作成
	drawMng_ = std::make_unique<DrawMng>(static_cast<float>(worldArea.y * tileSize.y));
	return false;
}

bool TutorialScene::InitGame(void)
{
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::stage, SoundProcess::GetBGMVolumeEntire(), true);

	// リザルトシーンに渡す結果を管理するクラスを作成
	resultData_ = std::make_shared<ResultData>();

	tmxObj_ = std::make_shared<TmxObj>();

	// マップの読み込み
	bool rtn{ tmxObj_->LoadTMX("./Resource/tmx/Tmap" + std::to_string(mapNum_) + ".tmx") };

	// ギミックのイベント用クラスを作成
	gEvent_ = std::make_shared<GmkEvent>(resultData_);

	// UIマネージャーを作成
	uiMng_ = std::make_shared<UIMng>();

	// Objマネージャーを作成
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
	// ギミックイベントにobjマネージャーをセット
	gEvent_->SetObjMng(objMng_);

	// 更新用関数を開始時の演出処理の方をセット
	updateFunc_ = std::bind(&TutorialScene::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);

	drawFunc_ = std::bind(&TutorialScene::DrawGame, this, std::placeholders::_1);

	// ゲーム開始前なのでfalseを
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
	// ここの部分は後で分けること
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
		// クリア時シーン移行
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
	// 自分自身のスクリーンに対してDraw
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
		// 座標をセット(プレイヤーのオフセット含めた座標)
		buff_->pos = objMng_->GetPlayer().lock()->GetPos() + camera_.GetOffset() + Math::Vector2{ 50.0f,60.0f };

		// 半径セット
		buff_->radius = 120.0f;

		// アラート時の値セット
		buff_->redValue = gEvent_->Color();

		// ピクセルシェーダセット
		SetUsePixelShader(psH_);

		// 定数バッファ更新
		UpdateShaderConstantBuffer(cbuffH_);

		// 定数バッファセット
		SetShaderConstantBuffer(cbuffH_, DX_SHADERTYPE_PIXEL, 0);

		// fogのテクスチャセット
		SetUseTextureToShader(1, lpImageMng.GetID("fog")[0]);

		// 描画
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
	// 自分自身のスクリーンに対してDraw
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
	// 描画用オフセットを取得
	const auto drawOffset = camera_.GetOffset();

	DrawGraph(static_cast<int>(drawOffset.x), static_cast<int>(drawOffset.y), mapFloor_, true);
	// マップの表示
	const auto& worldArea = tmxObj_->GetWorldArea();
	const auto& tileSize = tmxObj_->GetTileSize();

	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			// グラフィックidの取得
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

