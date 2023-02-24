#pragma once

class Obj;

// 座標をチェックする
struct CheckPosFunc
{
	bool operator()(Obj& obj, float delta);
};

// 敵の角度をチェックする
struct EnemyCheckAngleFunc
{
	bool operator()(Obj& obj, float delta);
};

enum class EnemyState;
// 敵の状態をチェックする
struct EnemyCheckStateFunc
{
	EnemyCheckStateFunc(EnemyState state);
	bool operator()(Obj& obj, float delta);
private:
	const EnemyState ckState_;
};

// 敵の状態が一定時間経ったかをチェックする
struct EnemyIsOverStateTimeFunc
{
	EnemyIsOverStateTimeFunc(float limitTime);
	bool operator()(Obj& obj, float delta);
private:
	const float limitTime_;
};

// 視界の判定が当たってるか
struct FanColIsHitFunc
{
	bool operator()(Obj& obj, float delta);
};

// 持っているNaviが目的地を見つけたかチェックする
struct NaviStateCheckFindFunc
{
	bool operator()(Obj& obj, float delta);
};

// Naviがsearch中でも見つけてもないとき
struct NaviCheckDestFunc
{
	bool operator()(Obj& obj, float delta);
};

// 巡回が停止中でないとき
struct PatrolCheckNotIsStop
{
	bool operator()(Obj& obj, float delta);
};

// 移動できるか?
struct IsMoveFunc
{
	bool operator()(Obj& obj, float delta);
};


