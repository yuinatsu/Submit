#include "../../../Object/Unit/Enemy.h"
#include "../../Navi/Navi.h"
#include "../../Navi/PatrolNavi.h"
#include "../../Collider/FanCollider.h"
#include "ConditionNodeFunc.h"

bool CheckPosFunc::operator()(Obj& obj, float delta)
{
	// 範囲をチェック
	return obj.GetPos().x > 0.0f && obj.GetPos().x < 1920.0f && obj.GetPos().y > 0.0f && obj.GetPos().y < 720.0f;
}

bool EnemyCheckAngleFunc::operator()(Obj& obj, float delta)
{
	auto& enemy = dynamic_cast<Enemy&>(obj);

	// 角度をチェックする
	auto angle = Math::GetNormalizedAngle(Math::Square(obj.GetMoveVec().GetAngle() - enemy.targetAngle_));
	if (angle  <= Math::Square(enemy.rotSpeed_))
	{
		// 差が一定値以下の時true
		return true;
	}
	return false;
}



bool NaviStateCheckFindFunc::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// ナビの状態が目的地を見つけたとき
	return navi.GetNaviState() == NaviState::Find;
}

bool NaviCheckDestFunc::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// ナビの状態が目的地につくもしくは何もないとき
	return navi.GetNaviState() == NaviState::Non;
}


bool PatrolCheckNotIsStop::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi));

	// ナビがストップではないときtrue
	return !navi.IsStop();
}

bool FanColIsHitFunc::operator()(Obj& obj, float delta)
{
	// ファン型のコライダーを取得
	auto& c = dynamic_cast<FanCollider&>(obj.GetComponent(ComponentID::Collider));
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	return c.HitCheck(enemy->targetPos_);
}

bool IsMoveFunc::operator()(Obj& obj, float delta)
{
	// 移動できるかを返す
	return obj.IsMove(delta);
}


EnemyIsOverStateTimeFunc::EnemyIsOverStateTimeFunc(float limitTime) :
	limitTime_{limitTime}
{
}

bool EnemyIsOverStateTimeFunc::operator()(Obj& obj, float delta)
{
	// 敵を取得
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	// リミットタイムを越したらtrue
	return enemy->GetStateTime() > limitTime_;
}


EnemyCheckStateFunc::EnemyCheckStateFunc(EnemyState state) :
	ckState_{state}
{
}

bool EnemyCheckStateFunc::operator()(Obj& obj, float delta)
{
	// 敵を取得
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	// チェックするステートと同じだったらtrue
	return enemy->GetEnemyState() == ckState_;
}
