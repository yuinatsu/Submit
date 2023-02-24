#include "Enemy.h"
#include "../../Component/Collider/CircleCollider.h"
#include "../../common/Debug.h"
#include "../ObjManager.h"
#include "../../Component/Collider/FanCollider.h"
#include "../../Component/Navi/Navi.h"
#include "../../Component/Navi/PatrolNavi.h"
#include "../../Component/Animation.h"
#include "../../common/GmkEvent.h"
#include "../../Component/BehaviorTree/BehaviorTree.h"

#include "../../Component/Navi/RandomNavi.h"
#include "../../common/SoundPross.h"

// 通常時のスピード
constexpr float speed{ 1.75f };

// 追跡時のスピード
constexpr float runSpeed{13.0f};

// 視野の角度
constexpr float vangle{ Math::Deg2Rad(45.0f) };

// 視野の長さ
constexpr float viewL{ 120.0f };

// コライダーとかのindex
constexpr int fanCol = 0;
constexpr int bodyCol = 1;
constexpr int soundCol = 2;

#ifdef _DEBUG
// 以下デバッグ用
constexpr const char* DebugStateStr(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Trace:
		return "Trace";
	case EnemyState::Rota:
		return "Rota";
	case EnemyState::Gimic:
		return "Gimic";
	case EnemyState::Patrol:
		return "Patrol";
	case EnemyState::MovePatrol:
		return "MovePatrol";
	case EnemyState::Non:
		return "Non";
	case EnemyState::Item:
		return "Item";
	case EnemyState::Lost:
		return "Lost";
	case EnemyState::Look:
		return "Look";
	case EnemyState::Hit:
		return "Hit";
	case EnemyState::Attack:
		return "Attack";
	case EnemyState::Sound:
		return "Sound";
	default:
		return "Non";
	}
}
#else
#define DebugStateStr() ()
#endif

Enemy::Enemy(ObjManager& objMng, std::weak_ptr< GmkEvent>& gmkEvent,std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos) :
	Unit{pos,tmxObj}, objMng_{objMng}, gmkEvent_{gmkEvent}
{
	stateTime_ = 0.0f;
	
	isIdle_ = false;
	moveVec_ = { -1.0f, 0.0f };
	state_ = EnemyState::Non;

	
	speed_ = 120.0f;
	spMag_ = 1.0f;

	Init(route);
}

Enemy::~Enemy()
{
}


EnemyState Enemy::GetEnemyState(void) const
{
	return state_;
}

void Enemy::SetRotationAngle(float angle)
{
	targetAngle_ = angle;
	Math::Vector2 targetVec = { std::cos(targetAngle_), std::sin(targetAngle_) };
	angleSing_ = Math::Cross(moveVec_, targetVec) > 0.0f ? 1.0f : -1.0f;

}

void Enemy::SetRotationAngle(void)
{
	targetAngle_ = (-moveVec_).GetAngle();
	Math::Vector2 targetVec = { std::cos(targetAngle_), std::sin(targetAngle_) };
	angleSing_ = Math::Cross(moveVec_, targetVec) > 0.0f ? 1.0f : -1.0f;
}

void Enemy::Rotation(double delta)
{
	dynamic_cast<FanCollider&>(GetComponent(ComponentID::Collider )).AddAngle(rotSpeed_ * angleSing_);
	auto angle = moveVec_.GetAngle() + rotSpeed_ * angleSing_;
	moveVec_ = { std::cos(angle), std::sin(angle) };
}

void Enemy::SetState(EnemyState state)
{
	state_ = state;
	stateTime_ = 0.0f;
}

void Enemy::CorrectMoveVecToAnim(void)
{
	
	auto ckFunc = [this](const Math::Vector2& rV, const Math::Vector2& lV)
	{
		if (Math::Cross(moveVec_, rV) < 0.0f &&  Math::Cross(moveVec_, lV) > 0.0f )
		{
			return true;
		}
		return false;
	};

	if (ckFunc(Math::downVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::leftVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Down");
		return;
	}

	if (ckFunc(Math::upVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::rightVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Up");
		return;
	}

	if (ckFunc(Math::downVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::leftVector2<float>))
	{

		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Left");
		return;
	}

	if (ckFunc(Math::upVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::rightVector2<float>))
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetState("Right");
		return;
	}
	
}

const float Enemy::GetStateTime(void) const
{
	return stateTime_;
}

void Enemy::Destroy(void)
{
	// 攻撃を受けた状態にする
	SetState(EnemyState::Hit);

	// アイドル状態にする
	SetIdleFlag(true);

	// 巡回を止める
	dynamic_cast<PatrolBaseNavi&>(GetComponent(ComponentID::PatrolNavi)).Stop();

	// リセットかける
	dynamic_cast<BehaviorTree&>(GetComponent(ComponentID::BehaviorTree)).Reset();
}

bool Enemy::StartMoveNavi(const Math::Vector2& pos)
{
	// ステータスチェック
	if (state_ == EnemyState::Look || state_ == EnemyState::Trace)
	{
		// 発見時もしくは追跡時はNaviで移動しない
		return false;
	}

	// Naviを取得
	auto& navi = dynamic_cast<Navi&>(GetComponent(ComponentID::Navi));

	// 引数で受けた座標をゴールに探索開始
	navi.Search(pos_, pos);
	SetState(EnemyState::Gimic);
	return true;
}

bool Enemy::StartMoveNavi(void)
{
	// targetPosへ向けてのNaviでの移動を開始
	return StartMoveNavi(targetPos_);
}

void Enemy::TraceStart(void)
{
	if (tracePlayerFlag_)
	{
		// プレイヤーを見つけている時
		if (!gmkEvent_.expired())
		{
			// gmkEventにアラート開始を伝える
			gmkEvent_.lock()->StartEvent(targetPos_, EventType::Alert);
		}
	}
}

bool Enemy::Init(std::vector<Math::Vector2I>& route)
{
	// 視界の判定を追加
	auto angle = moveVec_.GetAngle();
	Math::Vector2 v1{ std::cos(angle - vangle / 2.0f), std::sin(angle - vangle / 2.0f) };
	Math::Vector2 v2{ std::cos(angle + vangle / 2.0f), std::sin(angle + vangle / 2.0f) };
	v1 *= viewL;
	v2 *= viewL;
	bool rtn = AddComponent(std::make_unique<FanCollider>(tmx_, v1, v2, CheckType::EnemyFan,
		std::bind(&Enemy::HitFun, this, std::placeholders::_1)
		, *this
		)
	);

	// 登録した視界のコライダーを取得
	auto& col = dynamic_cast<Collider&>(GetComponent(ComponentID::Collider));

	// プレイヤー本体の当たり判定とデコイとアルコール相手に当たるようにする
	col.SetTargetType(static_cast<int>(CheckType::Alcohol) | static_cast<int>(CheckType::PlayerBody) | static_cast<int>(CheckType::Decoy));

	

	// Animationクラス追加
	rtn &= AddComponent(std::make_unique<Animation>("Resource/Xml/enemyAnim.xml", "Enemy", "Down", *this));

	// アニメーション追加
	rtn &= AddComponent(std::make_unique<Animation>("Resource/Xml/enemyReactionAnim.xml", "EnemyReaction", "Non", *this));

	// 経路探索用クラスをセット
	rtn = AddComponent(std::make_unique<Navi>(tmx_, *this));


	// 巡回用クラスをセット
	rtn &= AddComponent(std::make_unique<PatrolNavi>(route, tmx_, *this));

	// 本体用コライダーを追加
	rtn &= AddComponent(std::make_unique<CircleCollider>(
		10.0f,
		CheckType::EnemyBody,
		std::bind(&Enemy::HitBody,this,std::placeholders::_1),
		*this)
	);

	// 本体の当たり判定にアルコール、プレイヤーの本体、デコイ、地雷、プレイヤーの攻撃と当たるように
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider, bodyCol)).SetTargetType(
		static_cast<int>(CheckType::Alcohol) | static_cast<int>(CheckType::PlayerBody) |
		static_cast<int>(CheckType::Decoy) | static_cast<int>(CheckType::LandMine) | 
		static_cast<int>(CheckType::PlayerAttack) | static_cast<int>(CheckType::Box)
	);


	rtn &= AddComponent(std::make_unique<CircleCollider>(30.0f, CheckType::EnemySound, std::bind(&Enemy::HitSound,this,std::placeholders::_1), *this));

	// 音を探知する当たり判定にプレイヤーの本体と当たるように
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider, soundCol)).SetTargetType(static_cast<int>(CheckType::Sound));
	return rtn;
}

void Enemy::Update(float delta)
{
	stateTime_ += delta;

	// 視界を現在向いている方向に修正
	auto angle = moveVec_.GetAngle();
	Math::Vector2 v1{ std::cos(angle - vangle / 2.0f), std::sin(angle - vangle / 2.0f) };
	Math::Vector2 v2{ std::cos(angle + vangle / 2.0f), std::sin(angle + vangle / 2.0f) };
	dynamic_cast<FanCollider&>(GetComponent(ComponentID::Collider)).SetAngleFromVec(v1, v2);

	UpdateComponent(delta * spMag_);
	
}

constexpr Math::Vector2 reactionOffset{ 5.0f,-25.0f };

void Enemy::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{

	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, isIdle_);
	if (state_ == EnemyState::Look || state_ == EnemyState::Lost || state_ == EnemyState::Sound)
	{
		// LookかLostかSoundの時リアクションを表示
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator, 1)).Draw(drawMng, pos_ + reactionOffset + offset);
	}

}

void Enemy::HitBody(Collider& other)
{

	if (state_ == EnemyState::Hit)
	{
		return;
	}
	if (state_ == EnemyState::Trace && other.GetOwner().GetObjID() == ObjID::Player && other.GetMyCheckType() == CheckType::PlayerBody)
	{
		// プレイヤーを死亡させる処理
		other.GetOwner().Destroy();

	}
	if (other.GetOwner().GetObjID() == ObjID::Decoy)
	{
		// デコイの時の処理
		other.GetOwner().Destroy();
		SetState(EnemyState::Attack);
		objMng_.AddEffect(EffectName::Particle, pos_);
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator, 1)).SetState("Lost");
		return;
	}
	if (other.GetOwner().GetObjID() == ObjID::Alcohol)
	{
		// アルコールアイテムの時の処理
		other.GetOwner().Destroy();
		SetState(EnemyState::Item);
		objMng_.AddEffect(EffectName::Poison, pos_);
		return;
	}
	if (other.GetOwner().GetObjID() == ObjID::LandMine)
	{
		// 地雷の時の処理
		other.GetOwner().Destroy();
		Destroy();
		objMng_.AddEffect(EffectName::Explosion, pos_);
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::explosion, SoundProcess::GetSEVolumeEntire(), false);
		return;
	}
	if (other.GetOwner().GetObjID() == ObjID::Clock)
	{
		other.GetOwner().Destroy();
		SetState(EnemyState::Item);
		objMng_.AddEffect(EffectName::Particle, pos_);
		return;
	}
}

void Enemy::HitFun(Collider& other)
{
	// ステートチェック(アイテムか攻撃をくらった時もしくは追跡中は関係ない)
	if (state_ == EnemyState::Item || state_ == EnemyState::Hit)
	{
		return;
	}
	if (!(state_ == EnemyState::Trace || state_ == EnemyState::Look))
	{
		SetState(EnemyState::Look);
		dynamic_cast<PatrolBaseNavi&>(GetComponent(ComponentID::PatrolNavi)).Stop();
	}
	tracePlayerFlag_ = other.GetOwner().GetObjID() == ObjID::Player ? true : false;
	targetPos_ = other.GetOwner().GetPos();
	moveVec_ = (other.GetOwner().GetPos() - pos_).Normalized();

}

void Enemy::HitSound(Collider& other)
{
	if (!(state_ == EnemyState::Trace || state_ == EnemyState::Item || state_ == EnemyState::Hit))
	{
		if (other.GetOwner().GetObjID() == ObjID::Clock && (state_ == EnemyState::Gimic || state_ == EnemyState::Sound || state_ == EnemyState::Rota))
		{
			return;
		}
		if (other.GetOwner().GetObjID() == ObjID::Player)
		{
			if (dynamic_cast<Unit&>(other.GetOwner()).IsIdle() || other.GetOwner().GetSpMag() <= 0.4f)
			{
				return;
			}
		}
		SetState(EnemyState::Sound);
		dynamic_cast<PatrolBaseNavi&>(GetComponent(ComponentID::PatrolNavi)).Stop();
		targetPos_ = other.GetOwner().GetPos();
		SetRotationAngle((other.GetOwner().GetPos() - pos_).GetAngle());
	}
}
