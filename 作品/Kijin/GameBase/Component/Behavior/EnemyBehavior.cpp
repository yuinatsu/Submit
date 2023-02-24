#include "EnemyBehavior.h"
#include "../Transform/Transform.h"
#include "../../Application.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/StageBehavior.h"
#include "../../Common/Debug.h"
#include "PlayerBehavior.h"
#include "EnemyBulletBehavior.h"
#include "PlayerAttackBehavior.h"
#include "../../Common/SoundPross.h"

// 発射位置の上方向のずらす量
constexpr float shotUpOffset{ 240.0f };

// 発射位置の前方向のずらす量
constexpr float shotForwardOffset{ 300.0f };

// 発射位置の右方向のずらす量
constexpr float shotRightOffset{ 60.0f };

// 弾を発射するまでの時間
constexpr float shotTimeMax{ 2.5f };

// 回転が必要な範囲(角度)
constexpr float rotDeff{ Square(Deg2Rad(15.0f)) };

// ノックバック時の初速
constexpr float v0{ 6.0f };

// ノックバック用の重力加速度
constexpr float gravity{ 9.80f * 3.0f };

// コンストラクタ
EnemyBehavior::EnemyBehavior()
{
	// デフォルトのスピードとして120を入れとく
	speed_ = 120.0f;
	update_ = nullptr;
	stopDistance_ = 0.0f;
	searchDistance_ = 0.0f;
	// デフォルトの振り向き時間として1を入れとく
	turnTime_ = 1.0f;
	Init();
}

// 更新処理
void EnemyBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stateTime_ += delta;

	// 重力処理
	Gravity(delta);

	(this->*update_)(objectManager, delta);
}

// カウンタ変数や時間用変数の初期化
void EnemyBehavior::Init(void)
{
	shotTime_ = 0.0f;
	damageCnt_ = 0;
	rotTime_ = 0.0f;
	stateTime_ = 0.0f;
	hitTime_ = 0.0f ;
	startRot_= 0.0f ;
	targetRot_ = 0.0f;
	hitCombo_ = 0;
}

// 有効時の開始処理
void EnemyBehavior::Begin(ObjectManager& objectManager)
{
	// 変数を初期化
	Init();

	// トランスフォームを取得
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Pos().y += 5.0f;

	// プレイヤーのトランスフォームを取得
	playerT_ = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	// コライダーを取得しヒット時の関数をセット
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBehavior::OnHit, this,std::placeholders::_1, std::placeholders::_2));

	// 更新用関数をセット
	update_ = &EnemyBehavior::UpdateMove;
}

// 終了処理
void EnemyBehavior::End(ObjectManager& objectManager)
{
	// コンポーネントが向こうになった時に呼ばれる
	auto stage = objectManager.GetComponent<StageBehavior>(stageID_);
	if (stage.IsActive())
	{
		// ステージが生きている時ステージの敵のカウントを減らさせる
		stage->SubEnemy();
	}
}

// 追跡前の状態の更新
void EnemyBehavior::UpdateSearch(ObjectManager& objectManager, float delta)
{
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(searchDistance_))
	{
		// 一定の距離になったら移動開始する
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
	}
}

// 射撃処理
void EnemyBehavior::Shot(float delta, ObjectManager& objectManager)
{
	shotTime_ += delta;
	if (shotTime_ >= shotTimeMax)
	{
		shotTime_ = 0.0f;

		// 前上右のベクトルを取得
		auto up = transform_->GetUp();
		auto forward = transform_->GetForward();
		auto right = transform_->GetRight();
		auto shotPos = transform_->GetPos();

		// 上方向と前方向にずらす
		shotPos += up * shotUpOffset;
		shotPos += forward * shotForwardOffset;

		// 右上方向にずらして作成
		auto id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos + right * shotRightOffset);
		objectManager.Begin(id);

		// 左上方向にずらして作成
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos - right * shotRightOffset);
		objectManager.Begin(id);

		// 右下方向にずらして作成
		shotPos += -up * 70.0f;
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_,  shotPos + right * 80.0f);
		objectManager.Begin(id);

		// 左下方向にずらして作成
		id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos - right * 80.0f);
		objectManager.Begin(id);

		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::enemyAttack, SoundProcess::GetVolume(), false);

		// サウンドの座標を取得
		//Set3DPositionSoundMem(shotPos), static_cast<int>(SoundProcess::SOUNDNAME_SE::enemyAttack));

		// サウンドが届く範囲を指定
		Set3DRadiusSoundMem(256.0f, static_cast<int>(SoundProcess::SOUNDNAME_SE::enemyAttack));
			
	}
}

// 追跡移動状態の処理
void EnemyBehavior::UpdateMove(ObjectManager& objectManager, float delta)
{
	// 回転処理
	Rotation(delta);

	// 移動
	transform_->Pos() += transform_->GetForward() * (delta * speed_);
	
	// 射撃処理
	Shot(delta, objectManager);

	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(stopDistance_))
	{
		// 一定の距離になったら停止状態へ
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateStop;
	}
	else if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(searchDistance_))
	{
		// 一定の距離以上になったら追跡前の状態へ
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateSearch;
	}
}

// 停止状態の処理
void EnemyBehavior::UpdateStop(ObjectManager& objectManager, float delta)
{
	// 回転
	Rotation(delta);

	// 射撃
	Shot(delta, objectManager);
	
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(stopDistance_))
	{
		// 一定以上の距離になったら移動状態へ
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
	}

}

// 攻撃を受けた後の状態
void EnemyBehavior::UpdateHit(ObjectManager& objectManager, float delta)
{
	// 後ろに下がる
	transform_->Pos() += knockBackVec_ * (speed_) * delta;
	if (collider_->IsGround() && stateTime_ >= hitTime_)
	{
		// 硬直時間分経ったら
		stateTime_ = 0.0f;
		hitCombo_ = -1;
		update_ = &EnemyBehavior::UpdateMove;
		DebugLog("硬直終了");
		// 3回攻撃を受けたら消滅
		if (damageCnt_ >= 3)
		{
			DebugLog("倒された");
			objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::enemyDestroy, SoundProcess::GetVolume(), false);
		}
	}
}

// 回転処理
void EnemyBehavior::Rotation(float delta)
{
	
	rotTime_ -= delta;
	if (rotTime_ >= 0.0f)
	{
		// 回転中だった時の処理
		auto rot = Learp(targetRot_, startRot_,rotTime_/ turnTime_);
		Vector3 lookVec{ std::cos(rot),0.0f, std::sin(rot) };
		transform_->LookAt(lookVec);
		return;
	}


	auto targetVec{ playerT_->GetPos() - transform_->GetPos()};
	auto forward{ transform_->GetForward() };
	auto rot = Square(std::atan2(targetVec.z, targetVec.x) - std::atan2(forward.z, forward.x));
	if (rot > rotDeff)
	{
		// 回転すべき時
		rotTime_ = turnTime_;
		startRot_ = std::atan2(forward.z, forward.x);
		targetRot_ = std::atan2(targetVec.z, targetVec.x);
	}
}

// 重力処理
void EnemyBehavior::Gravity(float delta)
{
	if (!collider_->IsGround())
	{
		// 空中にいる時
		jumpTime += delta;
		float val = (v0 *jumpTime) - (0.5f * gravity * Square(jumpTime));
		transform_->Pos() += transform_->GetUp() * (val);
		collider_->SetGravityPow(val);
	}
	else
	{
		jumpTime = 0.0f;
	}
	//DebugDrawString("x=", transform_->GetPos().x, "y=", transform_->GetPos().y, "z=", transform_->GetPos().z, "veclocity=", jumpTime);
}

// ヒット時の処理
void EnemyBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::PlayerAttack)
	{
		auto attack = objectManager.GetComponent<PlayerAttackBehavior>(col.GetOwnerID());
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		if (hitCombo_ == attack->GetComboNum())
		{
			// 同じ攻撃なので処理しない
			//DebugLog("同じ攻撃");
			return;
		}

		if (damageCnt_ >= 3)
		{
			// もうすでに死亡しているので処理しない
			return;
		}
		DebugLog("攻撃通る");

		// 攻撃を受けたときの処理
		//DebugLog("攻撃を受けた!EnemyCombo=", hitCombo_, "AttackCombo=", attack->GetComboNum(), update_ == &EnemyBehavior::UpdateHit);
		damageCnt_++;
		player->RiseSkillValue();
		player->AddCombo();

		update_ = &EnemyBehavior::UpdateHit;
		stateTime_ = 0.0f;
		hitCombo_ = attack->GetComboNum();

		jumpTime = 0.0f;
		collider_->SetGroundFlag(false);
		transform_->Pos().y += 2.0f;
		objectManager.StartHitStop(0.05f);
		auto playerTransform = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
		if (playerTransform.IsActive())
		{
			knockBackVec_ = (transform_->GetPos() - playerTransform->GetPos());
			knockBackVec_.y = 0.0f;
			knockBackVec_.Normalize();
		}
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::playerAttackHit,SoundProcess::GetVolume(),false);
	}
}
