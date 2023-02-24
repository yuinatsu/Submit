#pragma once
#include <memory>
#include "../../common/Math.h"
#include "../../common/Random.h"
#include "EnemyState.h"
#include "Unit.h"

class  Camera;
struct Fan;
class ObjManager;
class TmxObj;
class GmkEvent;
class Collider;

// 敵クラス
class Enemy :
	public Unit
{
public:
	Enemy(ObjManager& objMng, std::weak_ptr< GmkEvent>&,std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos);
	~Enemy();

	/// <summary> 敵の状態を取得 </summary>
	/// <param name=""></param>
	/// <returns> 敵の状態 </returns>
	EnemyState GetEnemyState(void) const;

	/// <summary> 敵の状態をセットする </summary>
	/// <param name="state"> セットしたい状態 </param>
	void SetState(EnemyState state);

	/// <summary> 回転角度を設定する </summary>
	/// <param name="angle"> 回転終了角度 </param>
	void SetRotationAngle(float angle);

	/// <summary> 適当に回転させる </summary>
	/// <param name=""></param>
	void SetRotationAngle(void);

	/// <summary> 回転させる </summary>
	/// <param name="delta"></param>
	void Rotation(double delta);

	/// <summary> 移動方向からアニメーションを切り替える </summary>
	/// <param name=""></param>
	void CorrectMoveVecToAnim(void) final;

	/// <summary> 現在のステートになってからの経過時間を取得する </summary>
	/// <param name=""></param>
	/// <returns> ステートの経過時間 </returns>
	const float GetStateTime(void) const;

	/// <summary> 破壊時の処理 </summary>
	/// <param name=""></param>
	void Destroy(void) final;

	/// <summary> 特定の方向に経路探索で移動する </summary>
	/// <param name="pos"> ゴール座標 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool StartMoveNavi(const Math::Vector2& pos);

	bool StartMoveNavi(void);

	/// <summary> 追跡を開始する </summary>
	/// <returns></returns>
	void TraceStart(void);

private:
	bool Init(std::vector<Math::Vector2I>& route);
	void Update(float delta) override; 
	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

	ObjID GetObjID(void) override { return ObjID::Enemy; };      // idの取得

	/// <summary> コライダーに渡すようの本体に当たった時の処理 </summary>
	/// <param name="other"> 当たった相手のコライダー </param>
	void HitBody(Collider& other);

	/// <summary> コライダーに渡す用の視界に当たった時の処理 </summary>
	/// <param name="other"> 当たった相手の処理 </param>
	void HitFun(Collider& other);

	/// <summary> コライダーに渡す用の音の判定に当たった時の処理 </summary>
	/// <param name="other"> 当たった相手の処理 </param>
	void HitSound(Collider& other);

	// オブジェクトを管理しているクラス
	ObjManager& objMng_;

	// イベントを管理しているクラス
	std::weak_ptr< GmkEvent> gmkEvent_;

	// 敵の状態
	EnemyState state_;

	// 振り向き速度
	const float rotSpeed_ = Math::Deg2Rad(3.0f);

	// 振り向く角度
	float targetAngle_ = 0.0f;

	// 振り向く左右どちらか
	float angleSing_ = 1.0f;

	// 現在のステートになってからの経過時間
	float stateTime_;

	// 追跡とかのターゲットの座標
	Math::Vector2 targetPos_{-1.0f,-1.0f};

	// プレイヤーを追っているかのフラグ
	bool tracePlayerFlag_{ true };

	friend struct EnemyCheckAngleFunc;
	friend struct FanColIsHitFunc;

};

