#pragma once
#include <string>
#include <functional>
#include "../Math.h"

// リザルトシーンで結果を表示するクラスの基底
class ResultDraw
{
public:

	/// <summary> コンストラクタ </summary>
	/// <param name="startTime"> 開始時間(〇〇秒後にみたいな) </param>
	/// <param name="pos"> 中心座標 </param>
	ResultDraw(float startTime, const Math::Vector2& pos);

	virtual ~ResultDraw();

	/// <summary> アップデート </summary>
	/// <param name="nowTime"> 経過時間 </param>
	void Update(const float& nowTime);

	/// <summary> 描画 </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> 描画開始したかどうか </summary>
	/// <param name=""></param>
	/// <returns> 描画開始していたらtrue </returns>
	const bool IsStartedDraw(void) const { return drawTime_ - fadeTime_ > 0.0f; }
protected:

	/// <summary> 何もしない描画用のアップデート </summary>
	/// <param name="nowTime"></param>
	void UpdateNon(const float& nowTime);

	/// <summary> 描画時用のアップデート </summary>
	/// <param name="nowTime"></param>
	void UpdateDraw(const float& nowTime);

	/// <summary> 描画する </summary>
	/// <param name=""></param>
	virtual void DrawOwn(void) = 0;

	// 描画処理
	std::function<void(void)> drawFunc_;

	// アップデート
	std::function<void(const float&)> updateFunc_;

	// 描画開始までの時間
	const float drawStartTime_;

	// 描画開始後からの経過時間
	float drawTime_;

	// 中心座標
	Math::Vector2 pos_;

	static constexpr float fadeTime_{ 0.5f };
};

