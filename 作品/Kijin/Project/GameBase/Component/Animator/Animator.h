#pragma once
#include "../ComponentBase.h"
#include <memory>
#include <map>
#include "../ComponentHandle.h"
#include "../Render/ModelRender.h"

// アニメーションの再生状態を表すクラス(モデルのハンドルは分け合ってintで持ってる)
class AnimationState
{
public:
	virtual ~AnimationState() {}
	virtual AnimationState* Update(AnimationState* state, ObjectManager& objectManager,float delta) = 0;

	// 再生時用初期化処理 
	virtual void Init(AnimationState* start, AnimationState* end) = 0;

	// 再生終了しているか 
	virtual bool IsEnd(void) = 0;

	// 再生時間をリスタート 
	void ReStart(void);

	// アニメーションをアタッチ 
	void Attach(void);

	// アタッチしたインデックスを取得 
	int GetAttachIdx(void);

	// アニメーションをデタッチ 
	void Detach();

	// モデルのハンドルをセット 
	void SetHandle(int handle);

	// モデルのハンドルを取得 
	const int GetHandle(void) const;

	// アニメーションのセットアップ 
	void SetUpAnim(int idx);

	// アニメーションインデックスを取得 
	int GetAnimIdx(void);

	
	// ブレンドかどうか 
	virtual bool IsBlend(void) = 0;

	// ループを行うかのフラグをセット 
	void SetLoopFlag(bool flag)
	{
		isLoop_ = flag;
	}

	// 再生時間の取得 
	virtual float GetPlayTime(void) { return playTime_; }

	// 超過分再生時間の取得 
	virtual float GetPlayTimeOver(void) { return playTimeOver_; }

	// 総再生時間(最初から最後までの)を取得 
	virtual float GetTotalTime(void) { return totalTime_; }

	virtual Vector3 GetMovePosition(void) { return pos_; }

protected:

	// ハンドル
	int handle_{-1};

	// アタッチしたときのインデックス
	int attachIdx_{-1};

	// アニメーションのインデックス
	int animIdx_{-1};

	// ループするか？
	bool isLoop_{false};

	// 総再生時間
	float totalTime_{0.0f};

	// 再生時間
	float playTime_{0.0f};

	// 再生時間超過分
	float playTimeOver_{ 0.0f };

	Vector3 pos_;
	Vector3 prePos_;
	Vector3 nowPos_;

	int moveAnimFrameIndex_;


};

// 通常再生時の状態
class NormalState :
	public AnimationState
{
	// 初期化処理 
	void Init(AnimationState* start, AnimationState* end) final;

	// 更新処理 
	AnimationState* Update(AnimationState* state, ObjectManager& objectManager, float delta) final;
	
	bool IsEnd(void) final;

	// ブレンドかどうか 
	bool IsBlend(void)
	{
		return false;
	}
};


// ブレンド再生時の状態
class BlendState :
	public AnimationState
{
public:

	// ブレンド時間のセット 
	void SetBlendTime(const float blendTime)
	{
		blendTime_ = blendTime;
	}
private:

	// 初期化処理 
	void Init(AnimationState* start, AnimationState* end) final;

	// ブレンドかどうか 
	bool IsBlend(void)
	{
		return true;
	}

	// 更新処理 
	AnimationState* Update(AnimationState* state, ObjectManager& objectManager, float delta) final;

	bool IsEnd(void) final;

	float GetTotalTime(void) final { return end_->GetTotalTime(); }
	float GetPlayTime(void) final { return end_->GetPlayTime(); }

	// ブレンド開始時の状態
	AnimationState* start_{nullptr};

	// ブレンド終了時の状態
	AnimationState* end_{nullptr};

	// ブレンドする時間
	float blendTime_{1.0f};
};


class Animator :
    public ComponentBase
{
public:
	ComponentID GetID(void) const override
	{
		return id_;
	}
	SetID(ComponentID::Animator, ComponentID::Animator)

	// idxのアニメーションのへ移行 
	void SetNextAnimation(int idx, float blendTime = 1.0f);

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;

	// アニメーションを追加 
	void AddAnimation(int anitIdx, bool isLoop = true);

	// アニメーションの状態を表すクラスを取得 
	AnimationState* GetAnimState(void) { return nowState_; }
private:
	// 現在のステートクラス
	AnimationState* nowState_{nullptr};

	// ブレンド時に使用するステートクラス
	std::unique_ptr<BlendState> blendState_;

	// 各indexごとにアニメーション状態のクラスを保持
	std::map<int,std::unique_ptr<AnimationState>> animStates_;

	// 描画クラス
	ComponentHandle<ModelRender> render_;

};



