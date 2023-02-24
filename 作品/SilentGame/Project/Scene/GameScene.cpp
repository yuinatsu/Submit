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

	// ゲームに必要な部分を初期化
	InitGame();

	// カメラを初期化
	camera_.Init( *controller_, isSmallMap_, tmxObj_, objMng_->GetPlayer(), objMng_->GetTarget());
	
	// スクリーン周りを初期化
	InitScreen();

	SetUseASyncLoadFlag(false);

	viewPos_ = { 50,60 };

	return true;
}

bool GameScene::InitScreen(void)
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


	
	

	return true;
}

bool GameScene::InitGame(void)
{
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::stage, SoundProcess::GetBGMVolumeEntire(), true);

	// リザルトシーンに渡す結果を管理するクラスを作成
	resultData_ = std::make_shared<ResultData>();

	tmxObj_ = std::make_shared<TmxObj>();


	// マップの読み込み
	bool rtn{ tmxObj_->LoadTMX("./Resource/tmx/map" + std::to_string(mapNum_) + ".tmx") };

	// ギミックのイベント用クラスを作成
	gEvent_ = std::make_shared<GmkEvent>(resultData_);

	// UIマネージャーを作成
	uiMng_ = std::make_shared<UIMng>();

	// Objマネージャーを作成
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

	// ギミックイベントにobjマネージャーをセット
	gEvent_->SetObjMng(objMng_);

	// 更新用関数を開始時の演出処理の方をセット
	updateFunc_ = std::bind(&GameScene::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);

	drawFunc_ = std::bind(&GameScene::DrawGame, this, std::placeholders::_1);

	// ゲーム開始前なのでfalseを
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
		// クリア時シーン移行
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
		// ゲームオーバー時シーン移行
		Relese();
		return std::make_unique<DeadTrans>(objMng_, camera_.GetOffset(), *drawMng_, camera_, std::move(ownScene), std::make_unique<ResultScene>(*resultData_, mapNum_, isSmallMap_, delta));
	}
	return ownScene;
}

void GameScene::DrawGame(float delta)
{
	// 自分自身のスクリーンに対してDraw
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
		// 座標をセット(プレイヤーのオフセット含めた座標)
		screenEffect_.pos= objMng_->GetPlayer().lock()->GetPos() + camera_.GetOffset() + Math::Vector2{ 50.0f,60.0f };

		

		// アラート時の値セット
		screenEffect_.redValue = gEvent_->Color();

		// ピクセルシェーダセット
		SetUsePixelShader(lpShaderMng.Get(psID_));

		*cbuff_ = screenEffect_;
		cbuff_->Update();

		// 定数バッファセット
		SetShaderConstantBuffer(cbuff_->GetHandle(), DX_SHADERTYPE_PIXEL, 0);

		// fogのテクスチャセット
		SetUseTextureToShader(1, lpImageMng.GetID("fog")[0]);

		// 描画
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
	// 自分自身のスクリーンに対してDraw
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
			//auto image = y * worldArea.x + x;
			// グラフィックidの取得
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