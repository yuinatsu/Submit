#pragma once
//#include "Enemy.h"
class Enemy;
namespace Battle {
	class Player
	{
		Enemy* enemy_;// 4バイト or 8バイト(固定)
		int life_ = 100;
	public:
		void OnDamage(int damage);
		void Attack(void);
	};
}

