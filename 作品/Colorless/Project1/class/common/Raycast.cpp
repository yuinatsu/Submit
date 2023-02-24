#include "Raycast.h"
#include "../../_debug/_DebugConOut.h"

bool Raycast::CheckCollision(Ray ray, CollisionColor col)
{
	bool result = false;
	auto [first, second, color] = col;
	auto nowColor = static_cast<int>(lpColorMng.GetNowColor()) - static_cast<int>(Color::lightBlue);
	if ((color == lpColorMng.GetNowColor() && !lpColorMng.GetItemFlag(static_cast<Color>(nowColor + static_cast<int>(Color::lightBlueItem)))) || color == Color::max)
	{
		return result;
	}
	Line points[4] =
	{
		{
			first,
			first + Vector2F{second.x,0}
		},
		{
			first + Vector2F{second.x,0},
			first + second
		},
		{
			first + second,
			first + Vector2F{0,second.y}
		},
		{
			first + Vector2F{0,second.y},
			first
		}
	};
	for (auto point : points)
	{
		result |= CheckRay(ray, point);
	}
	return result;
}

bool Raycast::CheckRay(Ray ray, Line line)
{
	auto checkCross = [](Vector2F& vec1, Vector2F& vec2) {
		return ((vec1.x * vec2.y) - (vec2.x * vec1.y));
	};

	Vector2F vecRay = ray.v;
	Vector2F&& vecLine = line.end - line.p;

	float crossRayLine = checkCross(vecRay, vecLine);
	if (crossRayLine == 0)
	{
		return false;
	}

	Vector2F v = Vector2F{ line.p.x - ray.p.x, line.p.y - ray.p.y };
	float cross_vRay = checkCross(v, vecRay);
	float cross_vLine = checkCross(v, vecLine);

	float ans1 = cross_vRay / crossRayLine;
	float ans2 = cross_vLine / crossRayLine;
	if (ans1 > 0.0f && ans1 <= 1.0f && ans2 > 0.0f && ans2 <= 1.0f)
	{
		return true;
	}
	return false;
}
