#include "../../../Object/Unit/Enemy.h"
#include "../../Navi/Navi.h"
#include "../../Navi/PatrolNavi.h"
#include "../../../Scene/SceneMng.h"
#include "../../../common/SoundPross.h"
#include "NodeFunc.h"

NodeState MoveFunc::operator()(Obj& obj, float delta)
{
	// 移動させる
	obj.MovePos(delta);

	// アイドル状態フラグをfalseにする(Unit継承時)
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Success;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState RotaFunc::operator()(Obj& obj, float delta)
{
	// 敵を回転させる
	dynamic_cast<Enemy&>(obj).Rotation(delta);
	return NodeState::Success;
}


NodeState NaviSetRoutePatrolFunc::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// 巡回地点をゴールに経路を探索する
	navi.Search( obj.GetPos(), dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi)).GetPos());
	return NodeState::Success;
}

NodeState NaviMoveFunc::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// ベクトルと正規化したベクトルを用意
	const auto&& moveVec = navi.GetNaviPos() - obj.GetPos();
	auto moveVecN = moveVec.Normalized();
	
	// 長さを比較
	if (moveVec.SqMagnitude() < Math::Square(obj.GetSpeed() * delta *  obj.GetSpMag()))
	{
		// 通常の移動ベクトルの方が長いときそのままナビの指示の座標へ
		obj.SetPos(navi.GetNaviPos());
		return NodeState::Success;
	}
	
	// 移動ベクトルをセット
	obj.SetMoveVec(moveVecN);

	// 移動する
	obj.SetPos(obj.GetPos() + (moveVecN * (obj.GetSpeed() * delta * obj.GetSpMag())));

	// アイドル状態フラグをfalseにする
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Success;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState PatrolMoveFunc::operator()(Obj& obj, float delta)
{
	// 巡回用のナビを取得
	auto& navi = dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi));

	// ベクトルと正規化したものを用意
	const auto&& moveVec = navi.GetMovePos() - obj.GetPos();
	auto moveVecN = moveVec.Normalized();

	// 長さを比較
	if (moveVec.SqMagnitude() < Math::Square(obj.GetSpeed() * delta * obj.GetSpMag()))
	{
		// 通常の移動ベクトルの方が長いときそのままナビの指示の座標へ
		obj.SetPos(navi.GetMovePos());
		return NodeState::Success;
	}

	// 移動ベクトルをセット
	obj.SetMoveVec(moveVecN);

	// 移動する
	obj.SetPos(obj.GetPos() + (moveVecN * (obj.GetSpeed() * delta * obj.GetSpMag())));

	// アイドル状態フラグをfalseにする
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Faild;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState NaviWaitSearchFunc::operator()(Obj& obj, float delta)
{
	// ナビを取得
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// 状態をチェック
	if (navi.GetNaviState() != NaviState::Search)
	{
		// search以外の時成功
		return NodeState::Success;
	}
	// search中なので処理中
	return NodeState::Now;
}


NodeState PatrolNaviStartFunc::operator()(Obj& obj, float delta)
{
	// 巡回用ナビを取得
	auto patrol = dynamic_cast<PatrolBaseNavi*>(&obj.GetComponent(ComponentID::PatrolNavi));
	if (patrol == nullptr)
	{
		return NodeState::Faild;
	}

	// 巡回を開始する
	patrol->Start();
	return NodeState::Success;
}

NodeState PatrolNaviStopFunc::operator()(Obj& obj, float delta)
{
	// 巡回用ナビを取得
	auto patrol = dynamic_cast<PatrolBaseNavi*>(&obj.GetComponent(ComponentID::PatrolNavi));
	if (patrol == nullptr)
	{
		return NodeState::Faild;
	}
	// 巡回を停止する
	patrol->Stop();
	return NodeState::Success;
}

NodeState AnimCorrectMoveVecToAnim::operator()(Obj& obj, float delta)
{
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Faild;
	}
	// アニメーションを補正する
	unit->CorrectMoveVecToAnim();
	return NodeState::Success;
}



NodeState EnemyRandRotaFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	enemy->SetRotationAngle();
	return NodeState::Success;
}



EnemyWaitStateTimeFunc::EnemyWaitStateTimeFunc(float inWaitTime) :
	waitTime_{inWaitTime}
{
}

NodeState EnemyWaitStateTimeFunc::operator()(Obj& obj, float delta)
{
	// 敵を取得
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}

	// 時間をチェックする
	if (enemy->GetStateTime() > waitTime_)
	{
		// 待機時間を越えていたら成功
		return NodeState::Success;
	}
	// アイドル状態にする
	enemy->SetIdleFlag(true);

	// 時間を越えてないので処理中
	return NodeState::Now;
}

EnemySetStateFunc::EnemySetStateFunc(EnemyState state) :
	state_{state}
{
}

NodeState EnemySetStateFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// 敵の状態をセットする
	enemy->SetState(state_);
	return NodeState::Success;
}

EnemyReactionAnimFunc::EnemyReactionAnimFunc(const std::string& name) :
	animName_{name}
{
}

NodeState EnemyReactionAnimFunc::operator()(Obj& obj, float delta)
{
	auto anim = dynamic_cast<Animation*>(&obj.GetComponent(ComponentID::Animator, 1));
	if (anim == nullptr)
	{
		return NodeState::Faild;
	}
	anim->SetState(animName_);
	return NodeState::Success;
}

RandLRPosFunc::RandLRPosFunc(float val) :
	valMax_{val}
{
}

NodeState RandLRPosFunc::operator()(Obj& obj, float delta)
{
	// ここでランダムに左右に揺らす
	auto pos = obj.GetPos();
	pos.x += Rand.GetRandom(-valMax_, valMax_);
	obj.SetPos(pos);
	return NodeState::Success;
}

NodeState DeleteObjFunc::operator()(Obj& obj, float delta)
{
	// オブジェクトを基底の方処理を使い削除させる
	obj.Obj::Destroy();
	return NodeState::Success;
}

NodeState EnemyTraceFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// 敵の状態をセットする
	enemy->TraceStart();
	return NodeState::Success;
}

NodeState EnemyStartNaviTargetFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// 敵の状態をセットする
	
	return enemy->StartMoveNavi() ? NodeState::Success : NodeState::Faild;
}

PlaySEFunc::PlaySEFunc(int seID) :
	se_{ static_cast<SOUNDNAME_SE>(seID)}
{
}

NodeState PlaySEFunc::operator()(Obj& obj, float delta)
{
	SoundProcess::PlayBackSound(static_cast<SoundProcess::SOUNDNAME_SE>(se_), SoundProcess::GetSEVolumeEntire (), false);
	return NodeState::Success;
}
