#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "WarpStar.h"
#include "Player.h"
#include "Planet.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(SceneManager* manager, Player* player)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mPlayer = player;

	mActivePlanet = nullptr;
	mActiveName = NAME::MAIN_PLANET;
	mStep = 0.0f;
}

void Stage::Init(void)
{
	MakeMainStage();
	MakeFallPlanet();
	MakeFlatPlanet();
	MakeLastPlanet();
	MakeSpecialPlanet();
	MakeWarpStar();
	MakeGoalStar();

	mStep = -1.0f;
}

void Stage::Update(void)
{

	// ���[�v�X�^�[
	for (auto s : mWarpStars)
	{
		s->Update();
	}

	// �f��
	for (auto& s : mPlanets)
	{
		s.second->Update();

		// ���Ƃ͈قȂ�f����������
		if (mActiveName != s.second->GetName())
		{
			// �f���̏d�͌����ɓ�������
			if (s.second->InRangeGravity(mPlayer->GetTransform()->pos))
			{
				// ���̃X�e�[�W�֑J��
				ChangeStage(s.second->GetName());
			}
		}
	}

	//// �Q�[���I�[�o�[����
	//if (mActivePlanet != nullptr)
	//{
	//	// �v���C���[���������ł��邱��
	//	if (mPlayer->IsAlive())
	//	{
	//		// �e�f�����Ƃ̎��S�͈͂ɓ����Ă��邩
	//		if (mActivePlanet->InRangeDead(mPlayer->GetTransform()->pos))
	//		{
	//			mPlayer->StartDead();
	//		}
	//	}
	//}

}

void Stage::Draw(void)
{

	// ���[�v�X�^�[
	for (auto s : mWarpStars)
	{
		s->Draw();
	}

	// �f��
	for (auto s : mPlanets)
	{
		s.second->Draw();
	}

}

void Stage::Release(void)
{

	// ���[�v�X�^�[
	for (auto s : mWarpStars)
	{
		s->Release();
		delete s;
	}
	mWarpStars.clear();

	// �f��
	for (auto s : mPlanets)
	{
		s.second->Release();
		delete s.second;
	}
	mPlanets.clear();

}

void Stage::ChangeStage(NAME type)
{

	mActiveName = type;

	// �Ώۂ̃X�e�[�W���擾����
	mActivePlanet = GetPlanet(mActiveName);

	// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�
	mPlayer->ClearCollider();
	mPlayer->AddCollider(mActivePlanet->GetTransform()->collider);

	// �d�͐���ɘf����n��
	mSceneManager->GetGravityManager()->ChangeActivePlanet(mActivePlanet);

	mStep = TIME_STAGE_CHANGE;

}

Planet* Stage::GetPlanet(NAME type)
{
	if (mPlanets.count(type) == 0)
	{
		return nullptr;
	}

	return mPlanets[type];
}

void Stage::MakeMainStage(void)
{

	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	planetTrans.scl = AsoUtility::VECTOR_ONE;
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet = new Planet(mSceneManager, name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeFallPlanet(void)
{

	// ���Ƃ����̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FALL_PLANET));
	float scale = 10.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -12000.0f, 4100.0f, 10200.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::FALL_PLANET;
	Planet* planet = new Planet(mSceneManager, name, Planet::TYPE::SPHERE, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(3000.0f);
	planet->SetDeadLength(500.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeFlatPlanet(void)
{

	Transform planetTrans;
	Planet* planet;
	NAME name;
	float size;
	VECTOR scale;

	// �@ GROUND�^�C�v(�d�͂�Y+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	size = 10.0f;
	planetTrans.scl = { size, size, size };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -18000.0f, 7000.0f, 15000.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_BASE;
	planet = new Planet(mSceneManager, name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(1300.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

	// �Ȍ�̘f���̃X�P�[��
	//------------------------------------------------------------------------------
	size = 3.0f;
	scale = { size, size, size };
	//------------------------------------------------------------------------------

	// �A TRANS_ROT�^�C�v(�d�͂���]������Y+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-10.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 16800.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT01;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// �B TRANS_ROT�^�C�v(�d�͂���]������Y+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-70.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 17900.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT02;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// �C TRANS_ROT�^�C�v(�d�͂���]������Y+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-45.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 19000.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT03;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// �D TRANS_ROT�^�C�v(�d�͂���]������Y+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(0.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 20100.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT04;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// �E TRANS_CAMERA_FIXED�^�C�v(�d�͂���]������Y+�����J�����Œ�)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_02));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(80.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 21200.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_FIXED01;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_CAMERA_FIXED, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// �F TRANS_CAMERA_FIXED�^�C�v(�d�͂���]������Y+�����J�����Œ�)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_02));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(180.0f));
	planetTrans.pos = { -18000.0f, 7600.0f, 21800.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_FIXED02;
	planet = new Planet(mSceneManager, name, Planet::TYPE::TRANS_CAMERA_FIXED, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeLastPlanet(void)
{

	// �Ō�̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::LAST_PLANET));
	float scale = 8.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -18000.0f, 7600.0f, 24000.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::LAST_STAGE;
	Planet* planet = new Planet(mSceneManager, name, Planet::TYPE::SPHERE, planetTrans);
	planet->Init();
	planet->SetGravityRadius(1100.0f);
	planet->SetDeadLength(0.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeSpecialPlanet(void)
{

	// ���ʂȘf���f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager->LoadModelDuplicate(ResourceManager::SRC::SPECIAL_PLANET));
	float scale = 8.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -6000.0f, -150.0f, -1500.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::SPECIAL_STAGE;
	Planet* planet = new Planet(mSceneManager, name, Planet::TYPE::ROAD, planetTrans);
	planet->Init();
	planet->SetGravityRadius(700.0f);
	planet->SetDeadLength(1000.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeWarpStar(void)
{

	Transform trans;
	WarpStar* star;

	// �e�X�g�f���ւ̃��[�v�X�^�[
	//------------------------------------------------------------------------------
	//trans.pos = { 0.0, 50.0f, -600.0f };
	//trans.scl = { 0.6f, 0.6f, 0.6f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(0.0f),
	//	AsoUtility::Deg2RadF(180.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);

	//star = new WarpStar(mSceneManager, mPlayer, trans);
	//star->Init();
	//mWarpStars.push_back(star);
	//------------------------------------------------------------------------------

	// ���Ƃ����f���ւ̃��[�v�X�^�[
	//------------------------------------------------------------------------------
	trans.pos = { -910.0f, 200.0f, 894.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-25.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	star = new WarpStar(mSceneManager, mPlayer, trans);
	star->Init();
	mWarpStars.push_back(star);
	//------------------------------------------------------------------------------


	// ���R�Șf���ւ̃��[�v�X�^�[
	//------------------------------------------------------------------------------
	trans.pos = { -11360.0f, 6200.0f, 9600.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-12.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	star = new WarpStar(mSceneManager, mPlayer, trans);
	star->Init();
	mWarpStars.push_back(star);
	//------------------------------------------------------------------------------

}

void Stage::MakeGoalStar(void)
{
}
