#pragma once


enum class ComponentID :
	unsigned int
{
	Base = 1,									// すべての基底
	BehaviorBase = Base | (1 << 1),				// オブジェクトの動き
	Animator = Base | (1 << 2),					// アニメーション
	Transform = Base | (1 << 3),				// トランスフォーム
	Collider = Base | (1 << 4),					// 当たり判定
	RenderBase = Base | (1 << 5),				// 描画の基底
	ModelRender = RenderBase | (1 << 6),		// modelの描画
	Info = Base |(1 << 7),						// オブジェクトの情報
	SoundSpeaker = Base | (1 << 8),				// サウンドの再生
	CapsuleRender = RenderBase | (1 << 9),		// カプセルの描画
	PlayerBehavior = BehaviorBase | (1 << 10),	// プレイヤーの動き
	MeshCollider = Collider | (1 << 11),		// メッシュでの当たり判定
	SphereCollider = Collider | (1 << 12),		// 球体での当たり判定
	CapsuleCollider = Collider | (1 << 13),		// カプセルでの当たり判定
	CharactorCollider = Collider | (1 << 14),	// キャラクターの当たり判定
	TerrainCollider = Collider | (1 << 15),		// 地形の当たり判定
	EnemyBehavior = BehaviorBase | (1 << 16),	// 敵の動き
	EnemyBulletBehavior = BehaviorBase | (1 << 17),		// 敵の弾の動き
	StageBehavior = BehaviorBase | (1 << 18),			// ステージの制御
	PlayerAttackBehavior = BehaviorBase | (1 << 19),	// プレイヤーの攻撃
	EffectRender = RenderBase | (1 << 20),				// エフェクトクラス
	EffectBehavior = BehaviorBase | (1 << 21),			// 一部エフェクトの制御用のクラス
	Max = ~Base
};


