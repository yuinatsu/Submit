#pragma once
//#include "Enemy.h"
class Enemy;
namespace Battle {
	class Player
	{
		Enemy* enemy_;// 4�o�C�g or 8�o�C�g(�Œ�)
		int life_ = 100;
	public:
		void OnDamage(int damage);
		void Attack(void);
	};
}

