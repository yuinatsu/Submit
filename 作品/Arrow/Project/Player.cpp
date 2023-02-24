#include "Player.h"
#include "Enemy.h"

using namespace Battle;

void Player::OnDamage(int damage)
{
	life_ -= damage;
}

void Player::Attack(void)
{
	enemy_->OnDamage(10);
}
