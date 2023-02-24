#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Collider/Collider.h"
#include "../Render/ModelRender.h"
#include "../Transform/Transform.h"

class PlayerAttackBehavior :
	public Behavior
{
public:
	PlayerAttackBehavior();
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	/// <summary>
	/// 攻撃時間をセット
	/// </summary>
	/// <param name="attackTime"></param>
	void SetAttackTime(float attackTime)
	{
		attackTime_ = attackTime;
	}

	/// <summary>
	/// 攻撃判定が有効になるまでの時間
	/// </summary>
	/// <param name="startTime"></param>
	void SetAttackStartTime(float startTime)
	{
		attackStartTime_ = startTime;
	}

	/// <summary>
	/// コンボの情報をセット
	/// </summary>
	/// <param name="combo"></param>
	void SetComboNum(const int combo)
	{
		combo_ = combo;
	}

	/// <summary>
	/// 再生カウントをセット
	/// </summary>
	/// <param name="combo"></param>
	void SetPlayTime(const float time)
	{
		animTime_ = time;
	}

	/// <summary>
	/// コンボ情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int GetComboNum(void) const
	{
		return combo_;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::PlayerAttackBehavior };
private:

	/// <summary>
	/// オブジェクトが有効になった時に呼ばれる処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー </param>
	void Begin(ObjectManager& objectManager) final;

	/// <summary>
	/// オブジェクトが無効になった時に呼ばれる処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー  </param>
	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// ヒット時の処理
	/// </summary>
	/// <param name="col"> 相手コライダー </param>
	/// <param name="objectManager"> オブジェクトマネージャー  </param>
	void OnHit(Collider& col, ObjectManager& objectManager);

	/// <summary>
	/// 破棄時の処理
	/// </summary>
	/// <param name="objManager"> オブジェクトマネージャー  </param>
	void Destory(ObjectManager& objManager) final;

	/// <summary>
	/// 攻撃が有効の時の処理
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー  </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateAttack(ObjectManager& objectManager, float delta);

	/// <summary>
	/// 攻撃が有効ではないときの更新
	/// </summary>
	/// <param name="objectManager"> オブジェクトマネージャー  </param>
	/// <param name="delta"> デルタタイム </param>
	void UpdateNonAttack(ObjectManager& objectManager, float delta);

	void (PlayerAttackBehavior::* update_)(ObjectManager& , float );

	// 攻撃中の時間
	float attackTime_;

	// 攻撃開始までの時間
	float attackStartTime_;

	// 自身の攻撃のコンボ(プレイヤーから渡される)	
	int combo_;

	// 再生カウント
	float animTime_;

	// コライダー
	ComponentHandle<Collider> collider_;

	// プレイヤーのモデル描画クラス
	ComponentHandle<ModelRender> playerModel_;

	// 自身のトランスフォーム
	ComponentHandle<Transform> transform_;

	// 当たり判定の基準にするframe名
	std::wstring frameName_;

	// 当たり判定用のframeのインデックス
	int freameIdx_;


};

