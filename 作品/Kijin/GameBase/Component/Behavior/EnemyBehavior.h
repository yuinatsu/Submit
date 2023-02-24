#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Transform/Transform.h"
#include "../Collider/CharactorCollider.h"

// 敵の動き用のクラス
class EnemyBehavior :
	public Behavior
{
public:
	EnemyBehavior();

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::EnemyBehavior };

	/// <summary>
	/// 移動スピードのセット
	/// </summary>
	/// <param name="speed"> スピード </param>
	void SetSpeed(const float speed)
	{
		speed_ = speed;
	}

	
	float GetEnemyPower(void){return Power;}

	/// <summary>
	/// 攻撃を受けた際の硬直時間をセット
	/// </summary>
	/// <param name="hitTime"> 硬直時間 </param>
	void SetHitTime(const float hitTime)
	{
		hitTime_ = hitTime;
	}

	/// <summary>
	/// 振り向き終了にかかる時間をセット
	/// </summary>
	/// <param name="turnTime"> 振り向き終わるまでの時間 </param>
	void SetTurnTime(const float turnTime)
	{
		turnTime_ = turnTime;
	}

	/// <summary>
	/// 停止する距離をセット
	/// </summary>
	/// <param name="distance"> 停止するまでの距離 </param>
	void SetStopDistance(const float distance)
	{
		stopDistance_ = distance;
	}

	/// <summary>
	/// ステージのオブジェクトのID
	/// </summary>
	/// <param name="id"> ステージオブジェクトのID </param>
	void SetStageID(const ObjectID& id)
	{
		stageID_ = id;
	}

	/// <summary>
	/// 敵の追跡開始までの距離をセット
	/// </summary>
	/// <param name="distance"> 追跡開始までの距離 </param>
	void SetSearchDistance(const float distance)
	{
		searchDistance_ = distance;
	}

	enum class State
	{
		Search,
		Move,
		Stop,
		Hit
	};

private:
	static constexpr float Power = 20.0f;

	/// <summary>
	/// 時間やカウンター用変数を初期化する
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	void Begin(ObjectManager& objectManager) final;

	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// 追跡前の探知状態の更新
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateSearch(ObjectManager& objectManager, float delta);

	/// <summary>
	/// 弾の発射処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	/// <param name="delta"> デルタタイム </param>
	void Shot(float delta, ObjectManager& objectManager);
	
	/// <summary>
	/// 追跡移動時の処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateMove(ObjectManager& objectManager, float delta);

	/// <summary>
	/// 停止状態の処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateStop(ObjectManager& objectManager, float delta);

	/// <summary>
	/// 攻撃を受けた状態の処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateHit(ObjectManager& objectManager, float delta);

	/// <summary>
	/// 回転の処理
	/// </summary>
	/// <param name="delta"> デルタタイム </param>
	void Rotation(float delta);

	/// <summary>
	/// 重力処理(ノックバック用)
	/// </summary>
	/// <param name="delta"> デルタタイム </param>
	void Gravity(float delta);

	// 自身のトランスフォーム
	ComponentHandle<Transform> transform_;

	// プレイヤーのトランスフォーム
	ComponentHandle<Transform> playerT_;

	// 自身のコライダー
	ComponentHandle<CharactorCollider> collider_;

	/// <summary>
	/// ヒット時の処理
	/// </summary>
	/// <param name="col"> 当たった相手のコライダー </param>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	// 移動スピード
	float speed_;

	// ジャンプのスピード
	float jumpTime;

	// 停止する距離
	float stopDistance_;

	// 探知開始する距離
	float searchDistance_;

	// 射撃用
	float shotTime_;

	// 被弾カウント
	int damageCnt_;

	// 受けた攻撃のコンボ
	int hitCombo_;

	// 現在の状態に遷移してからの時間
	float stateTime_;

	// 回転開始時の角度
	float startRot_;

	// 回転先の角度
	float targetRot_;

	// 回転するまでの時間
	float rotTime_;

	// 回転して振り向き終えるまでの時間
	float turnTime_;

	// 攻撃を受けた際の硬直時間
	float hitTime_;

	// 更新処理
	void (EnemyBehavior::* update_)(ObjectManager&, float);

	// ステージオブジェクトのID
	ObjectID stageID_;

	// ノックバック方向
	Vector3 knockBackVec_;
};

