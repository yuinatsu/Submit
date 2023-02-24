#pragma once
#include <DxLib.h>
#include <vector>
#include "Quaternion.h"

class SceneManager;
class Particle;

class ParticleGenerator
{
public:
	static constexpr int NUM_PARTICLE = 30;
	static constexpr float PARTICLE_XANGLE_RAD = DX_PI_F / 6.0f;
	static constexpr int NUM_POLYGON = 10;
	static constexpr float SPLT_ANGLE_DEG = 360 / NUM_POLYGON;

	static constexpr float GEN_MIN_SIZE = 1.0f;
	static constexpr float GEN_MAX_SIZE = 5.0f;

	static constexpr float GEN_MIN_TIME = 1.0f;
	static constexpr float GEN_MAX_TIME = 1.0f;

	static constexpr float GEN_MIN_SPEED = 10.0f;
	static constexpr float GEN_MAX_SPEED = 20.0f;

	static constexpr float GEN_SPEED = 0.1f;

	ParticleGenerator(SceneManager* manager,VECTOR pos,float radius);
	~ParticleGenerator();
	void Init(void);
	void Update(void);
	void Draw(void);
	void DebugProcess(void);
	void DrawMashSquere(void);
	void DrawMashCircle(void);
	void Release(void);

	void SortParticle(void);

	void SetPos(const VECTOR& pos);
	void SetRot(const Quaternion& qua);

private:
	SceneManager* sceneManager_;

	int imageLightHadle_;

	VECTOR pos_;
	float radius_;
	float parGen_;
	VERTEX3D vertexSquere[6];

	Quaternion quaternion_;

	std::vector<Particle*> particles_;

	VERTEX3D vertexCircle[NUM_POLYGON + 1];
	WORD indexs[NUM_POLYGON * 3];

	void CreateMeshSquere(void);
	void CreateMeshCircle(void);
	Particle* Generate(Particle* particle);
};

