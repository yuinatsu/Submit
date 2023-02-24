#pragma once
#include <string>
#include <deque>
#include <functional>
#include "Math.h"


// 描画用データ
struct DrawData
{
	Math::Vector2 pos;		// 描画座標
	float depth;			// 深度に関する値(0.0fが手前1.0fが奥として描画順を決めるのに使う)
	float rot;				// 回転角度
	int handle;				// 画像ハンドル
};

// 描画順番を管理するクラス
class DrawMng
{
public:
	DrawMng(float maxPosY);
	~DrawMng();

	/// <summary> 左上基準に描画する </summary>
	/// <param name="pos"> 左上座標 </param>
	/// <param name="handle"> 画像のハンドル </param>
	void Draw(const Math::Vector2& pos,int handle);

	/// <summary> 中心基準に描画する </summary>
	/// <param name="pos"> 中心座標 </param>
	/// <param name="rot"> 回転角度 </param>
	/// <param name="handle"> 画像のハンドル </param>
	void Draw(const Math::Vector2& pos, float rot, int handle);

	/// <summary> 中心基準に深度を指定して描画する </summary>
	/// <param name="pos"> 中心座標 </param>
	/// <param name="depth"> 深度(0.0fが手前1.0fが奥) </param>
	/// <param name="rot"> 回転角度 </param>
	/// <param name="handle"> 画像のハンドル </param>
	void Draw(const Math::Vector2& pos, float depth, float rot, int handle);

	/// <summary> キューにためていた描画処理を実行する </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// 描画の処理内容のfunctionと深度を指定して描画キューに入れる
	/// </summary>
	/// <param name="func"> 描画処理のfunction </param>
	/// <param name="depth"> 描画順を決めるための深度(0.0fが手前1.0fが奥) </param>
	void Draw(std::function<void(void)>&& func, float depth);

	/// <summary> y座標の位置から描画順を決める深度を求める </summary>
	/// <param name="y"></param>
	/// <returns></returns>
	const float GetPosY2Depth(const float y) const { return 1.0f - (y / maxPosY_); }
private:

	using DrawFunc = std::function<void(void)>;

	// 描画処理をデータにしてデキューで持つ
	std::deque<std::pair<DrawFunc, float>> drawQueue_;

	
	

	// 処理すべきキューの数
	int queueCount_;

	// Y座標の最大値
	const float maxPosY_;

};

