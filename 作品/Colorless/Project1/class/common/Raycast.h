#pragma once
#include "Vector2.h"
#include "Collision.h"

class Raycast
{
public:
	struct Ray 
	{
		Vector2F p;		// 始点
		Vector2F v;		// 方向ベクトル
	};
	struct Line 
	{
		Vector2F p;		// 始点
		Vector2F end;	// 終点
		Vector2F vec()	//始点から終点へのベクトル
		{
			return end - p;
		}
	};

	bool CheckCollision(Ray ray,CollisionColor col);
	bool CheckRay(Ray ray,Line line);
};

