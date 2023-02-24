#pragma once
#include <memory>
#include <chrono>
#include "../common/Vector2.h"

class BaseScene;

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance_;
		return s_Instance_;
	}

	void Run(void);

	const Vector2& GetScreenSize(void);

private:
	SceneMng();
	~SceneMng();

	bool SysInit(void);

	Vector2 screenSize_;
	std::chrono::system_clock::time_point now_;
	std::chrono::system_clock::time_point old_;
	std::unique_ptr<BaseScene> scene_;
};

