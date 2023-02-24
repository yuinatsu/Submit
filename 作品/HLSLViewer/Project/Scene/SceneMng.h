#pragma once
#include "BaseScene.h"
#include "../Common/Time.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	// 静的なSingleton
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	// 実行処理
	void Run(void);
private:
	// システム系初期化
	bool SysInit(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 終了処理
	void End(void);
	// 時間系のクラス
	TimeClass time_;
	// シーン情報
	SceneUptr scene_;
};

