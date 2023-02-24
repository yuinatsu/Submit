#pragma once
#include "Vector2.h"
#include "Collision.h"

class Raycast
{
public:
	struct Ray 
	{
		Vector2F p;		// �n�_
		Vector2F v;		// �����x�N�g��
	};
	struct Line 
	{
		Vector2F p;		// �n�_
		Vector2F end;	// �I�_
		Vector2F vec()	//�n�_����I�_�ւ̃x�N�g��
		{
			return end - p;
		}
	};

	bool CheckCollision(Ray ray,CollisionColor col);
	bool CheckRay(Ray ray,Line line);
};

