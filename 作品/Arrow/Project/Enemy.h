#pragma once
//#ifndef ENEMY_H_INCLUDED
//#define ENEMY_H_INCLUDED
//#include "Player.h"

// ネームスペース内のクラスの前方宣言はこのように、そのネームスペースで囲んじゃいます
namespace Battle {
	class Player;// プロトタイプ宣言
	class Weapon;
	class Shot;
}
class Enemy
{
	Battle::Player* player_;
	Battle::Weapon* weapon_;
	Battle::Shot* shot_;
	int life_ = 100;
public:
	void OnDamage(int damage);
	void Attack(void);
};
//#endif

