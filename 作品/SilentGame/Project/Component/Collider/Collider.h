#pragma once
#include <functional>
#include <type_traits>
#include "../Component.h"

class Obj;
class CircleCollider;
class FanCollider;
class Collider;
using GetHitFunc = std::function<void(Collider&)>;


enum class CheckType : int
{
	Non = 0,			// 何も当たらない
	EnemyBody = 1 << 0,		// 敵の本体の当たり判定
	PlayerBody = 1 << 1,		// プレイヤーの本体の当たり判定
	Box = 1 << 2,			// box
	Alert = 1 << 3,			// アラート
	TrapAlarm = 1 << 4,		// トラップ
	SnareFloor = 1 << 5,	// 音がする床
	LandMine = 1 << 6,		// 地雷
	Alcohol = 1 << 7,		// アルコール
	Decoy = 1 << 8,		// デコイ
	Item = 1 << 9,			// アイテム
	EnemyFan = 1 << 10,	// 敵の視界
	EnemySound = 1 << 11,	// 敵の聴力
	PlayerAttack = 1 << 12,// 敵の攻撃判定
	Target = 1 << 13,
	Sound = 1 << 14,
	SecurityCam = 1 << 15,
	Door = 1 << 16,
	All = ~Non			// すべてと当たる
};

// 当たり判定用クラスの基底クラス
class Collider :
	public Component
{
public:

	/// <summary> コンストラクタ </summary>
	/// <param name="getHitFunc"> 当たった時の処理をfunctionで </param>
	/// <param name="owner"> 持ち主 </param>
	Collider(CheckType targetType,const GetHitFunc& getHitFunc,Obj& owner);
	virtual ~Collider();

	/// <summary> 対象のコライダーと当たっているかを調べる処理 </summary>
	/// <param name="col"> 対象のコライダー </param>
	virtual void HitCheck(Collider& col) = 0;

	virtual bool HitBoolCheck(Collider& col) = 0;

	/// <summary> 対象のコライダーと当たっているかを調べる処理(押し出しもする) </summary>
	/// <param name="col">対象のコライダー</param>
	virtual bool HitExCheck(Collider& col) = 0;

	/// <summary> サークルコライダーとの当たり判定を調べる </summary>
	/// <param name="col"> サークルコライダー </param>
	/// <returns></returns>
	virtual bool HitCheck(CircleCollider& col) = 0;

	/// <summary> サークルコライダーとの当たり判定を調べる(押し出し) </summary>
	/// <param name="col">サークルコライダー</param>
	/// <returns></returns>
	virtual bool HitExCheck(CircleCollider& col) = 0;

	virtual bool HitCheck(FanCollider& col) = 0;

	/// <summary> 持ち主を取得 </summary>
	/// <returns> 持ち主(Objの参照) </returns>
	Obj& GetOwner() { return owner_; }

	/// <summary> 当たった時の反応処理を取得 </summary>
	/// <param name=""></param>
	/// <returns> std::functionの参照 </returns>
	GetHitFunc& GetFunc(void) { return getHitFunc_; }

	bool IsHit(void)const { return hitFlag_ && !oldHitFlag_; }
	
	bool IsHitStay(void) const { return hitFlag_ && oldHitFlag_; }

	bool IsHitRelese(void) const { return !hitFlag_ && oldHitFlag_; }

	void Hit(void) { hitFlag_ = true; }
	void HitFlagUpdate(void) { oldHitFlag_ = hitFlag_; }

	/// <summary> ターゲットタイプに含まれているか？ </summary>
	/// <param name="targetType"> 相手のタイプ </param>
	/// <returns> 含まれていたらtrueそうでなけらばfalse </returns>
	const bool IsTargetType(CheckType targetType) const
	{ 
		auto r = (static_cast<std::underlying_type<CheckType>::type>(targetType) & targetType_);
		return  r > 0;
	}

	/// <summary> 自身の種類を返す </summary>
	/// <param name=""></param>
	/// <returns> 自身のタイプ </returns>
	const CheckType GetMyCheckType(void) const { return myType_; }

	/// <summary> 当たり判定を取る相手のタイプをセットする </summary>
	/// <param name="targetType"> 取りたい相手のタイプをintにして|とかで複数指定できる </param>
	void SetTargetType(int targetType) { targetType_ = targetType; }
protected:
	bool hitFlag_;
	bool oldHitFlag_;

	/// <summary> 当たった時の処理をするfunction </summary>
	GetHitFunc getHitFunc_;

	// 自身のタイプ
	CheckType myType_;

	// 当たり判定を取りたい相手
	std::underlying_type<CheckType>::type targetType_ = static_cast<std::underlying_type<CheckType>::type>(CheckType::All);
private:

	/// <summary> IDを取得する </summary>
	/// <returns> ID </returns>
	ComponentID GetID() const override;
};

