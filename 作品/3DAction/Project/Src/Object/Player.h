#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
#include "Stage.h"
class SceneManager;
class ResourceManager;
class GravityManager;
class AnimationController;
class Capsule;
class Collider;

class Player
{

public:

	static constexpr float SPEED_MOVE = 5.0f;

	static constexpr float SPEED_RUN = 10.0f;

	static constexpr float TIME_ROT = 1.0f;

	static constexpr float JUMP_POW = 35.0f;

	static constexpr float JUMP_TIME_IN = 0.5f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	Player(SceneManager* manager);
	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);

	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);

	void Release(void);

	// 操作
	void ProcessMove(void);
	void ProcessJump(void);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 傾斜の計算
	void CalcSlope(void);

	// 移動量の計算
	void CalcGravityPow(void);

	void AddCollider(Collider* collider);
	void ClearCollider(void);

	Transform* GetTransform(void);

	// 状態別更新ステップ
	void UpdateWarpReserve(void);
	void UpdateWarpMove(void);
	Capsule* GetCapsule(void);

	// 状態判定
	bool IsPlay(void);
	bool IsWarpMove(void);
	bool IsAlive(void);

	// ワープ開始
	void StartWarp(float time, Quaternion goalRot, VECTOR goalPos);

	// ゲームオーバー状態
	void StartDead(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// アニメーション
	AnimationController* mAnimationController;

	// 状態
	STATE mState;

	// 移動スピード
	float mSpeed;

	// 移動方向
	VECTOR mMoveDir;

	// 移動量
	VECTOR mMovePow;

	// 移動後の座標
	VECTOR mMovedPos;

	// 回転
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	// ジャンプ量
	VECTOR mJumpPow;

	// ジャンプ判定
	bool mIsJump;

	// ジャンプ入力受付時間
	float mStepJump;

	// 衝突判定に用いられるコライダ
	Capsule* mCapsule;
	std::vector<Collider*> mColliders;

	// 衝突チェック
	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	// 丸影
	int mImgShadow;

	// 足煙エフェクト
	int mEffectSmoke;
	int mStepFootSmoke;

	// フレームごとの移動量
	VECTOR mMoveDiff;

	// ワープ用
	float mTimeWarp;
	float mStepWarp;

	// ワープ準備完了時の情報
	Quaternion mWarpQua;
	VECTOR mWarpReservePos;

	// ワープ準備開始時のプレイヤー情報
	Quaternion mReserveStartQua;
	VECTOR mReserveStartPos;

	// ワープ前の惑星名
	Stage::NAME mPreWarpName;

	// 軌跡エフェクト
	int mEffectWarpOrbit;
	int mHandleWarpOrbitL;
	int mHandleWarpOrbitR;

	// モデルパーツのフレーム番号
	int mFrameLeftHand;
	int mFrameRightHand;

	// 傾斜関係
	VECTOR mSlopeDir;
	float mSlopeAngleDeg;
	VECTOR mSlopePow;
	VECTOR mHitNormal;
	VECTOR mHitPos;

	// 状態遷移
	void ChangeState(STATE state);

	// 衝突判定
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	// 着地モーション終了
	bool IsEndLanding(void);

	// 足煙エフェクト
	void EffectFootSmoke(void);

	// 軌跡エフェクト
	void EffectWarpOrbit(void);
	void SyncWarpOrbitPos(void);
	void StopOrbitEffect(void);
};

