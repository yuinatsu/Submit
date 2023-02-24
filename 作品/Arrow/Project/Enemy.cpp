#include "Enemy.h"
#include "Player.h"

void Enemy::OnDamage(int damage)
{
	life_ -= damage;
}

void Enemy::Attack(void)
{
	player_->OnDamage(10);
}
