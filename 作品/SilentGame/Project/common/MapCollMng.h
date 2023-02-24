#pragma once
#include <deque>
#include <tuple>
#include "Math.h"

using Collision = std::pair<Math::Vector2, Math::Vector2>;			// 始点　,　幅と高さ
using ColData = std::deque<Collision>;

class MapCollMng
{
public:
	struct Ray
	{
		Math::Vector2 p;		// 始点
		Math::Vector2 vec;		// 方向ベクトル
	};
	struct Line
	{
		Math::Vector2 p;		// 始点
		Math::Vector2 end;		// 終点
	};
	static std::pair<bool, Math::Vector2> CheckCollision(Ray ray, Collision col);


	/// <summary> レイとコリジョン情報から当たったかどうかのフラグと当たったところまでのベクトルとコリジョンのベクトル(正規化)を返す </summary>
	/// <param name="ray"> レイ </param>
	/// <param name="col"> コリジョン </param>
	/// <returns></returns>
	static std::tuple<bool, Math::Vector2, Math::Vector2> CheckColl2(Ray& ray, Collision& col);
	static std::pair<bool, Math::Vector2> CheckRay(Ray ray, Line line);
private:
};

