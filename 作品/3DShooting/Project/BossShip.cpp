#include "BossShip.h"
#include "SceneManager.h"
#include "EventShot.h"
#include "Camera.h"
#include "Turret.h"
#include "Player.h"
#include "AsoUtility.h"

namespace
{
	constexpr float SPEED_MOVE = 4.0f;
	constexpr float EVENT_TIME = 2.0f;
}

namespace
{
	constexpr float EXP_RADIUS_X = 100.0f;
	constexpr float EXP_RADIUS_Y = 100.0f;
	constexpr float EXP_RADIUS_Z = 400.0f;
}

BossShip::BossShip(SceneManager* manager, Player* player) :
	sceneManager_(manager), player_(player)
{
}

void BossShip::Init(void)
{
	VECTOR pos = { 0.0f,0.0f,4000.0f };
	transform_.SetModel(MV1LoadModel("Model/BossShip/BossShip.mv1"));
	transform_.scl = { 2.0f,2.0f,2.0f };
	transform_.pos = pos;
	transform_.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(210.0f), AsoUtility::Deg2RadF(20.0f));
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	transform_.Update();

	// �O��
	MakeTurret(
		{ 4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(-18.0f) });
	MakeTurret(
		{ -4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(18.0f) });

	// ��
	MakeTurret(
		{ 4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(90.0f), 0.0f });
	MakeTurret(
		{ -4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f });

	// ���
	MakeTurret(
		{ 3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(18.0f) });
	MakeTurret(
		{ -3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(-18.0f) });

	MV1SetupCollInfo(transform_.modelId);

	SceneManager::SCENE_ID id = sceneManager_->GetmSceneID();
	switch (id)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChengeState(BOSS_STATE::EVENT);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		ChengeState(BOSS_STATE::BATTLE);
		break;
	default:
		break;
	}
}

void BossShip::Update(void)
{
	switch (state_)
	{
	case BOSS_STATE::EVENT:
		auto nor = VNorm(transform_.GetForward());
		auto moveVec = VScale(nor, SPEED_MOVE);
		transform_.pos = VAdd(transform_.pos, moveVec);

		eventShot_->Update();
		if (!eventShot_->IsAlive())
		{
			stepEvent_ -= sceneManager_->GetDeltaTime();
			if (stepEvent_ < 0.0f)
			{
				sceneManager_->ChangeScene(SceneManager::SCENE_ID::BATTLE,true);
				return;
			}
		}
		UpdateTurret();
		break;
	case BOSS_STATE::BATTLE:
		VECTOR dir = transform_.GetForward();
		transform_.pos = VAdd(transform_.pos, VScale(dir, SPEED_MOVE));
		float speed = (DX_PI_F / 180.0f) * sceneManager_->GetDeltaTime();
		Quaternion que = Quaternion::AngleAxis(speed, AsoUtility::AXIS_Y);
		transform_.quaRot = Quaternion::Mult(transform_.quaRot, que);
		UpdateTurret();
		int cnt = 0;
		for (const auto& turret : turretList_)
		{
			if (turret->IsAlive())
			{
				cnt++;
			}
		}
		if (cnt == 0)
		{
			ChengeState(BOSS_STATE::EXP);
		}
		break;
	}
	transform_.Update();

	MV1RefreshCollInfo(transform_.modelId);
}

void BossShip::UpdateTurret(void)
{
	for (auto turret : turretList_)
	{
		turret->Update();
	}
}

void BossShip::Draw(void)
{
	switch (state_)
	{
	case BOSS_STATE::EVENT:
		MV1DrawModel(transform_.modelId);
		eventShot_->Draw();
		DrawTurret();
		break;
	case BOSS_STATE::BATTLE:
		MV1DrawModel(transform_.modelId);
		DrawTurret();
		break;
	case BOSS_STATE::EXP:
		MV1DrawModel(transform_.modelId);
		DrawTurret();
		
		break;
	default:
		break;
	}
	for (const auto& pos : expPosList_)
	{
		//DrawSphere3D(pos, 1.0f, 16, 0xffffff, 0xffffff, true);
	}
}

void BossShip::DrawTurret(void)
{
	for (auto turret : turretList_)
	{
		turret->Draw();
	}
}

void BossShip::Release(void)
{
	MV1DeleteModel(transform_.modelId);
	if (eventShot_ != nullptr)
	{
		eventShot_->Release();
		delete eventShot_;
	}
	for (auto turret : turretList_)
	{
		turret->Release();
		delete turret;
	}
	turretList_.clear();
	expPosList_.clear();
}

std::vector<Turret*> BossShip::GetTurret(void)
{
	return turretList_;
}

const int& BossShip::GetModelID(void)
{
	return transform_.modelId;
}

void BossShip::MakeTurret(VECTOR localPos, VECTOR localAngle)
{
	Turret* turret = new Turret(sceneManager_, player_->GetTransForm(), &transform_, localPos, localAngle);
	turret->Init();

	turretList_.emplace_back(turret);
}

void BossShip::ExpInit(void)
{
	float angleX = AsoUtility::Deg2RadD(360.0f / 30.0f);
	float angleY = AsoUtility::Deg2RadD(360.0f / 40.0f);
	while (angleX <= DX_TWO_PI_F)
	{
		while (angleY <= DX_TWO_PI_F)
		{
			
			Quaternion rot = Quaternion::AngleAxis(angleY, AsoUtility::AXIS_Y);
			rot = rot.Mult(Quaternion::AngleAxis(angleX, AsoUtility::AXIS_X));
			VECTOR tmpPos = Quaternion::PosAxis(rot, transform_.pos);
			tmpPos = VNorm(tmpPos);
			tmpPos.x *= EXP_RADIUS_X;
			tmpPos.y *= EXP_RADIUS_Y;
			tmpPos.z *= EXP_RADIUS_Z;
			expPosList_.emplace_back(tmpPos);
			angleY += AsoUtility::Deg2RadD(360.0f / 80.0f);
		}
		angleY = AsoUtility::Deg2RadD(360.0f / 80.0f);
		angleX += AsoUtility::Deg2RadD(360.0f / 60.0f);
	}
}

void BossShip::ChengeState(BOSS_STATE state)
{
	if (state_ == state)
	{
		return;
	}
	state_ = state;
	
	switch (state_)
	{
	case BossShip::BOSS_STATE::NON:
		break;
	case BossShip::BOSS_STATE::EVENT:
		stepEvent_ = EVENT_TIME;
		eventShot_ = new EventShot(sceneManager_, &transform_);
		auto norVec = VNorm(VSub(sceneManager_->GetCamera()->GetPos(), transform_.pos));
		eventShot_->Create(transform_.pos, norVec);
		break;
	case BossShip::BOSS_STATE::BATTLE:
		ExpInit();
		break;
	case BossShip::BOSS_STATE::EXP:

		break;
	case BossShip::BOSS_STATE::END:
		break;
	default:
		break;
	}
}
