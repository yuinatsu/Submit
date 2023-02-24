#pragma once
#include <utility>
#include "Vector2.h"

class Raycast
{
public:
	struct Ray
	{
		Vector2Flt point;
		Vector2Flt vec;
	};
	struct Line
	{
		Line(Vector2Flt p, Vector2Flt e)
		{
			point = p;
			end = e;
		}
		Vector2Flt point;
		Vector2Flt end;
		Vector2Flt vec()
		{
			return end - point;
		}
	};
	bool CheckCollision(const Ray& ray, std::pair<Vector2, Vector2> col);
	bool CheckLine(Ray ray, Line line);
};