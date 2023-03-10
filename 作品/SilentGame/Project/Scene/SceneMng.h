#pragma once
#include "../common/Time.h"
#include "../common/Random.h"
#include "BaseScene.h"

#define lpSceneMng SceneMng::GetInstance()
#define Rand (lpSceneMng.GetRandom())

class SceneMng
{
public:
	// 静的シングルトン
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	// 実行処理
	void Run(void);

	const Math::Vector2I& GetScreenSize(void)const;
	const Math::Vector2I& GetCameraSize(void)const;
	Random& GetRandom(void) { return random_; }
private:
	// 初期化処理
	bool SysInit(void);
	void Update(void);
	void Draw(float delta);
	bool InitFlag_;
	TimeClass time_;
	Random random_;
	uniqueScene scene_;

	SceneMng();
	~SceneMng();
};

