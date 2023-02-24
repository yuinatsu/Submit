#pragma once
#include <memory>
#include <unordered_map>
#include "../Component/ComponentID.h"
#include "../common/Math.h"
#include "../input/Controller.h"
#include "../common/MapCollMng.h"
#include "../Component/Animation.h"
#include <string>
#include <deque>
#include "Effect/EffectID.h"
#include "Item/ItemName.h"

class Component;

using ComponentUPTr = std::unique_ptr<Component>;
using Components = std::deque<ComponentUPTr>;
using ComponentMap = std::unordered_map<ComponentID, Components>;

// プレイヤーの状態
struct Move;
struct SpChange;
struct CheckKey;
struct SetAnime;
struct ColisionCheck;

// 描画処理用マネージャー
class DrawMng;

/// <summary> オブジェクトの種類 </summary>
enum class ObjID
{
	Player,				// プレイヤー
	Enemy,				// 敵
	UI,					// UI
	Box,				// アイテムが入ってるBox
	Alert,				// アラート
	TrapAlarm,			// トラップアラーム
	SnareFloor,			// 音がなる床
	LandMine,			// 地雷
	Alcohol,			// アルコール
	Decoy,				// デコイ
	Effect,				// エフェクト
	Clock,				// 時計
	Target,				// クリア目標
	Decoration,			// 装飾用
	DoorL,				// 開閉扉大
	DoorS				// 開閉扉小
};

class Obj
{
public:
	Obj(const Math::Vector2& pos);
	virtual ~Obj();

	virtual void Update(float delta);
	virtual void Draw(const Math::Vector2& offset, DrawMng& drawMng);

	/// <summary> コンポーネントを取得する </summary>
	/// <param name="id"> 取得したいコンポーネントのID </param>
	/// <returns> コンポーネントの参照 </returns>
	const Components& GetAllComponent(ComponentID id) const;

	/// <summary> コンポーネントを取得する </summary>
	/// <param name="id"> 取得したいコンポーネントのID </param>
	/// <returns> コンポーネントの参照 </returns>
	Component& GetComponent(ComponentID id, int idx = 0) const;

	/// <summary> IDに該当するコンポーネントが含まれるか </summary>
	/// <param name="id"> 探したいコンポーネントのID </param>
	/// <returns></returns>
	bool IncludeComponent(ComponentID id) const;

	/// <summary> コンポーネントを追加する </summary>
	/// <param name="component"> 追加したいコンポーネント </param>
	/// <returns> 成功時true失敗時false </returns>
	bool AddComponent(ComponentUPTr&& component);

	/// <summary> 座標を取得 </summary>
	/// <param name=""></param>
	/// <returns> 座標 </returns>
	const Math::Vector2& GetPos(void) const;

	/// <summary> 座標をセット </summary>
	/// <param name="pos"> セットしたい座標 </param>
	void SetPos(const Math::Vector2& pos);

	/// <summary> ObjID取得 </summary>
	/// <returns> ObjID </returns>
	virtual ObjID GetObjID(void) = 0;

	/// <summary> 移動方向ベクトルをセットする </summary>
	/// <param name="moveVec"></param>
	void SetMoveVec(const Math::Vector2& moveVec);

	/// <summary> 移動方向ベクトルを取得する </summary>
	/// <param name=""></param>
	/// <returns> 移動方向ベクトル </returns>
	const Math::Vector2& GetMoveVec(void) const;

	/// <summary> moveVec、speed_、spMag_、deltaを使い移動する </summary>
	/// <param name="delta"></param>
	virtual void MovePos(float delta);

	/// <summary> 移動できるかどうか？ </summary>
	/// <returns> 移動できるときtrueできないときfalse </returns>
	virtual const bool IsMove(float delta) const;

	/// <summary> スピードを取得する </summary>
	/// <param name=""> スピード </param>
	/// <returns></returns>
	const float GetSpeed(void) const;

	/// <summary> プレイヤー移動倍率セット </summary>
	/// <param name="spMag"> 移動スピード倍率 </param>
	void SetSpMag(float spMag);

	/// <summary> スピードの倍率を取得する </summary>
	/// <returns> スピードの倍率 </returns>
	const float GetSpMag(void) const;

	/// <summary> 生存しているか？ </summary>
	/// <returns> 生存しているときtrueしていないときfalse </returns>
	const bool IsAlive(void) const { return alive_; }

	/// <summary> このオブジェクトを破棄するために生存フラグをfalseにする(オブジェクト固有の処理をしてからにしたい場合はオーバーライドしてね) </summary>
	virtual void Destroy(void) { alive_ = false; }

	/// <summary> 発生させるエフェクト情報取得 </summary>
	/// <returns> EffectName </returns>
	EffectName GetEffect(void) const { return effect_; }

	/// <summary> 攻撃中フラグ </summary>
	/// <returns> 攻撃中:true、否:false </returns>
	const bool IsAttack(void) const { return isAtk_; }

	/// <summary> 攻撃フラグセット </summary>
	/// <param name="flag"> 攻撃中フラグ </param>
	void SetAttackFlag(bool flag) { isAtk_ = flag; }
protected:

	/// <summary> コンポーネントの更新処理 </summary>
	/// <param name="delta"></param>
	void UpdateComponent(float delta) const;

	/// <summary> 座標 </summary>
	Math::Vector2 pos_;

	/// <summary> 移動方向 </summary>
	Math::Vector2 moveVec_;

	/// <summary> 移動量 </summary>
	float speed_;

	/// <summary> 移動量倍率 </summary>
	float spMag_;

	/// <summary> デルタタイム </summary>
	float delta_;

	/// <summary> 生存フラグ </summary>
	bool alive_;

	/// <summary> 攻撃中フラグ </summary>
	bool isAtk_;

	/// <summary> 発生させるエフェクト </summary>
	EffectName effect_;

	ItemName item_;

	// フレンド
	friend Move;
	friend SpChange;
	friend CheckKey;
	friend SetAnime;
	friend ColisionCheck;
	friend struct MovePos;
	friend struct Attack;
private:
	/// <summary> コンポーネント類をマップで持つ </summary>
	ComponentMap componentList_;
};

