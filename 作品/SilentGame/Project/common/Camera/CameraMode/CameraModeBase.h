#pragma once

class Camera;

class CameraModeBase
{
public:
	CameraModeBase();
	virtual ~CameraModeBase();
	virtual void Update(Camera& camera,float delta) = 0;
	virtual void Draw(Camera& camera) = 0;
	virtual void Loaded(void){}
};

