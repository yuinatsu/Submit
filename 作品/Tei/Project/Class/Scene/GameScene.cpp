#include <DxLib.h>
#include <string>
#include "GameScene.h"
#include "TitleScene.h"
#include "Transition/CrossOver.h"
#include "Transition/FadeInOut.h"
#include "../Map/MapData.h"
#include "../Mng/ImageMng.h"
#include "../Mng/AnimationMng.h"
#include "../Mng/CollisionMng.h"
#include "../Object/Player.h"
#include "../collision/Collision.h"
#include "../collision/SquaerCollision.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

const SceneID GameScene::GetSceneID(void) const
{
	return SceneID::Game;
}

bool GameScene::Init(void)
{
	AnimInit();
	mapData_ = std::make_unique<MapData>("Resource/mapData/map.tmx");
	
	const auto& info = mapData_->GetMapInfo(MAP_LAYER::BLOCK);

	lpImageMng.GetID("map", info.imageStr, info.chipSize, info.imageSize / info.chipSize);

	objList_.emplace_back(std::make_shared<Player>(Vector2Flt{ 500.0f,100.0f }, Vector2{0,0},Object_ID::Pawn,10,TeamTag::RED,InputType::KEYBOARD));
	objList_.emplace_back(std::make_shared<Player>(Vector2Flt{ 500.0f,100.0f }, Vector2{ 0,0 }, Object_ID::Pawn, 10, TeamTag::BLUE, InputType::PAD));

	for (const auto& obj : objList_)
	{
		Vector2Flt size = static_cast<Vector2Flt>(obj->GetSize());
		auto col = std::make_shared<SquaerCollision>(size, size / 2.0f);
		col->SetOwner(obj);
		lpCollisionMng.RegistrationCol(col);
	}

	return true;
}

void GameScene::AnimInit(void)
{
	lpAnimMng.LoadAnimTmx("Resource/animData.xml", Object_ID::Pawn);
}

std::unique_ptr<BaseScene> GameScene::Update(const double& delta, std::unique_ptr<BaseScene> ownScene)
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<TitleScene>());
	}
	for (const auto& obj : objList_)
	{
		obj->Update(delta,mapData_);
	}
	lpCollisionMng.Update();
	return std::move(ownScene);
}

void GameScene::DrawOwnScreen(const double& delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	for (const auto layer:MAP_LAYER())
	{
		const auto& info = mapData_->GetMapInfo(layer);
		for (int y = 0; y < info.mapSize.y; y++)
		{
			for (int x = 0; x < info.mapSize.x; x++)
			{
				int id = mapData_->GetMapChipData(layer,x, y);
				if (id < 0)
				{
					continue;
				}
				DrawGraph(info.chipSize.x * x, info.chipSize.y * y, lpImageMng.GetID("map")[id], true);
			}
		}
	}
	for (const auto& obj : objList_)
	{
		obj->Draw(delta);
	}
}
