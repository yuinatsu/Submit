#include "Stage.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "Player.h"

namespace
{
	constexpr float grid_length = 3000.0f;
	constexpr float grid_term = 100.0;

	constexpr float ENEMY_EVENT_RADIUS = 10000;
	constexpr float ENEMY_EVENT_SQ_RADIUS = ENEMY_EVENT_RADIUS * ENEMY_EVENT_RADIUS;
}

Stage::Stage(SceneManager* manager, Player* player):
	mSceneManager(manager),player_(player)
{

}

void Stage::Init(void)
{
	stageModel_ = MV1LoadModel("Model/Rock/Dungeon.mv1");
	MV1SetScale(stageModel_, { 1.0f,1.0f,1.0f });
	MV1SetPosition(stageModel_, { 1000.0f,400.0f,1300.0f });
	MV1SetRotationXYZ(stageModel_, { 0,180.0f,0.0f });
	MV1SetupCollInfo(stageModel_);

	enemyModel_ = MV1LoadModel("Model/BossShip/BossShip.mv1");
	MV1SetScale(enemyModel_, { 2.0f,2.0f,2.0f });
	enemyPos_ = { 14500.0f,-800.0f,6500.0f };
	MV1SetPosition(enemyModel_, enemyPos_);
	MV1SetRotationXYZ(enemyModel_, { 0,240.0f,0.0f });
}

void Stage::Update(void)
{
	auto pPos = player_->GetTransForm()->pos;
	auto ePos = enemyPos_;

	auto diffVec = VSub(pPos, ePos);

	float diff = VSquareSize(diffVec);
	

	if (std::abs(diff) < ENEMY_EVENT_SQ_RADIUS)
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}
}

void Stage::Draw(void)
{
	DrawGrid();
	MV1DrawModel(stageModel_);
	MV1DrawModel(enemyModel_);
}

void Stage::Release(void)
{
	MV1DeleteModel(stageModel_);
	MV1DeleteModel(enemyModel_);
}

const int& Stage::GetModelHadle(void)
{
	return stageModel_;
}

void Stage::DrawGrid(void)
{
	int num = grid_length / grid_term;
	for (int i = -num; i < num; i++)
	{
		//X
		DrawLine3D(VGet(-grid_length, 0, grid_term * i), VGet(grid_length, 0, grid_term * i), 0xff0000);
		DrawLine3D(VGet(grid_term * i, 0, -grid_length), VGet(grid_term * i, 0, grid_length), 0x0000ff);
	}

	DrawLine3D(VGet(0, -grid_length, 0), VGet(0, grid_length, 0), 0x00ff00);
	DrawSphere3D(enemyPos_, ENEMY_EVENT_RADIUS, 128, 0xff0000, 0xff0000, false);
}

