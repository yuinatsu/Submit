#pragma once
#include "BaseScene.h"

// 設定画面等のシーン
class OptionScene :
    public BaseScene
{
public:
	OptionScene();

private:
	const SceneID GetID(void) const final
	{
		return SceneID::Option;
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta"></param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// シーンの描画
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

};

