#pragma once
#include <string>
#include "../../../Object/Unit/EnemyState.h"
#include "../NodeState.h"

class Obj;

enum class SOUNDNAME_SE;

// 移動
struct MoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 回転
struct RotaFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 敵を回転させる
struct EnemyRandRotaFunc
{
	NodeState operator()(Obj& obj, float delta);
};



// Naviの目的地を巡回地点にする
struct NaviSetRoutePatrolFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// Naviに従いObjを移動させる
struct NaviMoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// PatrolNaviに従いObjを移動させる
struct PatrolMoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// Naviの探索を待機する
struct NaviWaitSearchFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 巡回を開始する
struct PatrolNaviStartFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 巡回を停止する
struct PatrolNaviStopFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// MoveVecをもとにアニメーションをセットする
struct AnimCorrectMoveVecToAnim
{
	NodeState operator()(Obj& obj, float delta);
};


// 敵の状態時間が指定時間になるまで待つ処理
struct EnemyWaitStateTimeFunc
{
	EnemyWaitStateTimeFunc(float inMaxTime);
	NodeState operator()(Obj& obj, float delta);
private:
	const float waitTime_;
};

// 敵の状態をセットする処理
struct EnemySetStateFunc
{
	EnemySetStateFunc(EnemyState state);
	NodeState operator()(Obj& obj, float delta);
private:
	const EnemyState state_;

};

// 敵の反応用アニメーションをセットする処理
struct EnemyReactionAnimFunc
{
	EnemyReactionAnimFunc(const std::string& name);
	NodeState operator()(Obj& obj, float delta);
private:
	const std::string animName_;
};

// ランダムに左右に揺れる処理
struct RandLRPosFunc
{
	RandLRPosFunc(float val);
	NodeState operator()(Obj& obj, float delta);
private:
	const float valMax_;
};

// オブジェクトを破棄する処理
struct DeleteObjFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 敵が追跡時の処理
struct EnemyTraceFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// 敵が持つターゲット座標へNaviを開始する
struct EnemyStartNaviTargetFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// SEの再生する処理
struct PlaySEFunc
{
	PlaySEFunc(int seID);
	NodeState operator()(Obj& obj, float delta);
	SOUNDNAME_SE se_;
};

