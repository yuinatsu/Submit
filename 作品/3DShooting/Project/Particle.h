#pragma once
#include <DxLib.h>

class SceneManager;

class Particle
{
public:
	Particle(SceneManager* scenemanager,int imageHandleID);
	~Particle();

	void Generate(VECTOR pos, float size, VECTOR dir, float speed, float limitTime);
	void Update(void);
	void Draw(void);
	void Release(void);

	const VECTOR& GetPos(void)const;

	const float& GetZlen(void)const;
	void SetZlen(const float& zlen);
	bool isAlive(void);
private:
	SceneManager* sceneManager_;
	int imageHandle_;
	VECTOR pos_;
	VECTOR dir_;
	float size_;
	float speed_;
	float limitTime_;

	float zlen_;
};

