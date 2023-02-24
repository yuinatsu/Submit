#include <cmath>
#include <algorithm>
#include "ParticleGenerator.h"
#include "Particle.h"
#include "Quaternion.h"
#include "SceneManager.h"
#include "Camera.h"
#include "AsoUtility.h"
#include "RandomEngine.h"

ParticleGenerator::ParticleGenerator(SceneManager* manager, VECTOR pos, float radius):
	sceneManager_(manager),pos_(pos),radius_(radius)
{
	//imageLightHadle_ = -1;
	parGen_ = 0.0f; 
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::Init(void)
{
	imageLightHadle_ = LoadGraph("Image/Light.png");
	//CreateMeshSquere();
	CreateMeshCircle();
}

void ParticleGenerator::Update(void)
{
	DebugProcess();
	if (particles_.size() < NUM_PARTICLE && parGen_ >= GEN_SPEED)
	{
		particles_.emplace_back(Generate(nullptr));
		parGen_ = 0.0f;
	}
	for (auto& par : particles_)
	{
		par->Update();
		if (!par->isAlive())
		{
			par = Generate(par);
		}
	}
	SortParticle();
	parGen_ += sceneManager_->GetDeltaTime();
}

void ParticleGenerator::Draw(void)
{
	//DrawBillboard3D(pos_, 0.5f, 0.5f, 100.0f, 0.0f, imageLightHadle_, true);

	//DrawMashSquere();
	//DrawMashCircle();
	for (const auto& par : particles_)
	{
		par->Draw();
	}
}

void ParticleGenerator::DebugProcess(void)
{
	Quaternion tmpQ = {};
	bool isHitKey = false;
	if (CheckHitKey(KEY_INPUT_L))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f), AsoUtility::AXIS_Y);
	}
	if (CheckHitKey(KEY_INPUT_J))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(-1.0f), AsoUtility::AXIS_Y);
	}
	if (CheckHitKey(KEY_INPUT_I))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f), AsoUtility::AXIS_X);
	}
	if (CheckHitKey(KEY_INPUT_K))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(-1.0f), AsoUtility::AXIS_X);
	}
	if (CheckHitKey(KEY_INPUT_U))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f), AsoUtility::AXIS_Z);
	}
	if (CheckHitKey(KEY_INPUT_O))
	{
		isHitKey = true;
		tmpQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(-1.0f), AsoUtility::AXIS_Z);
	}
	if (isHitKey)
	{
		quaternion_ = quaternion_.Mult(tmpQ);
	}
}

void ParticleGenerator::DrawMashSquere(void)
{
	DrawPolygon3D(vertexSquere, 2, DX_NONE_GRAPH, true);
	for (const auto& vertex:vertexSquere)
	{
		DrawSphere3D(vertex.pos,5,16,0xff0000,0xff0000,true);
	}
}

void ParticleGenerator::DrawMashCircle(void)
{
	DrawPolygonIndexed3D(vertexCircle, NUM_POLYGON + 1, indexs, NUM_POLYGON, DX_NONE_GRAPH, true);
	for (const auto& vertex : vertexCircle)
	{
		DrawSphere3D(vertex.pos, 5, 16, 0xff0000, 0xff0000, true);
	}
}

void ParticleGenerator::Release(void)
{
}

void ParticleGenerator::SortParticle(void)
{
	Camera* camera = sceneManager_->GetCamera();
	VECTOR cDir = camera->GetDir();
	VECTOR cPos = camera->GetPos();
	VECTOR pDir = {};
	for (const auto& par : particles_)
	{
		pDir = VSub(par->GetPos(), cPos);
		par->SetZlen(VDot(cDir, pDir));
	}

	std::sort(particles_.begin(), particles_.end(),
		[](const Particle* parA, const Particle* parB) {return parA->GetZlen() > parB->GetZlen(); });

}

void ParticleGenerator::SetPos(const VECTOR& pos)
{
	pos_ = pos;
	CreateMeshCircle();
}

void ParticleGenerator::SetRot(const Quaternion& qua)
{
	quaternion_ = qua;
}



void ParticleGenerator::CreateMeshSquere(void)
{
	COLOR_U8 c = GetColorU8(0xff, 0xff, 0xff, 0xff);
	vertexSquere[0].pos = {0,0,100};
	vertexSquere[1].pos = {100,0,100};
	vertexSquere[2].pos = {0,0,0};
	vertexSquere[3].pos = { 0,0,0 };
	vertexSquere[4].pos = { 100,0,100 };
	vertexSquere[5].pos = { 100,0,0 };
	
	for (auto& vertex : vertexSquere)
	{
		vertex.dif = c;
		vertex.spc = c;
	}
}

void ParticleGenerator::CreateMeshCircle(void)
{
	COLOR_U8 c = GetColorU8(0xff, 0xff, 0xff, 0xff);
	VECTOR cPos = pos_;
	cPos.y += 0.1;
	vertexCircle[0].pos = cPos;
	VECTOR pos;
	Quaternion rot;
	float cntAngle = 0.0f;
	for (int i = 0; i < NUM_POLYGON; i++)
	{
		rot = Quaternion::Mult(quaternion_, Quaternion::AngleAxis(AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y));
		pos = VECTOR{ 0.0f,0.0f,radius_ };
		pos = Quaternion::PosAxis(rot, pos);

		vertexCircle[i + 1].pos = VAdd(cPos,pos);
		cntAngle += SPLT_ANGLE_DEG;
	}
	
	for (int i = 0; i < NUM_POLYGON; i++)
	{
		indexs[i * 3] = 0;
		indexs[i * 3 + 1] = i + 1;
		indexs[i * 3 + 2] = (i + 2) < NUM_POLYGON + 1 ? i + 2 : 1;
	}

	for (auto& vertex : vertexCircle)
	{
		vertex.dif = c;
		vertex.spc = c;
	}
	vertexCircle[0].dif = GetColorU8(0xff, 0x00, 0x00, 0xff);
}

Particle* ParticleGenerator::Generate(Particle* particle)
{
	if (particle == nullptr)
	{
		particle = new Particle(sceneManager_, imageLightHadle_);
	}
	float angle = RandomEngine::RandomFloat(0.0f, DX_TWO_PI_F);
	float rad = RandomEngine::RandomFloat(radius_ / 2.0f, radius_);
	Quaternion rot = Quaternion::Mult(quaternion_, Quaternion::AngleAxis(angle, AsoUtility::AXIS_Y));
	VECTOR pos = VECTOR{ RandomEngine::RandomFloat(-radius_, radius_) ,0.0f,RandomEngine::RandomFloat(-radius_, radius_) };
	pos = Quaternion::PosAxis(rot, pos);

	VECTOR dir = { 0.0f,0.0f,0.0f };
	Quaternion xRot = Quaternion::AngleAxis(RandomEngine::RandomFloat(-PARTICLE_XANGLE_RAD, PARTICLE_XANGLE_RAD) - DX_PI_F / 2.0f, AsoUtility::AXIS_X);

	Quaternion finalRot = Quaternion::Mult(rot, xRot);

	dir = finalRot.GetForward();
	float size = RandomEngine::RandomFloat(GEN_MIN_SIZE, GEN_MAX_SIZE);
	float limitTime = RandomEngine::RandomFloat(GEN_MIN_TIME, GEN_MAX_TIME);
	float speed = RandomEngine::RandomFloat(GEN_MIN_SPEED, GEN_MAX_SPEED);

	particle->Generate(VAdd(pos, pos_), size, dir, speed, limitTime);
	return particle;
}
