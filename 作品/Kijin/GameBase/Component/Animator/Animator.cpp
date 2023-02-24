#include "Animator.h"
#include <DxLib.h>
#include "../../Object/ObjectManager.h"

#include "../../Common/Debug.h"

void Animator::SetNextAnimation(int idx, float blendTime)
{
	if (nowState_ == nullptr)
	{
		// 現在のステートがないときとりあえずidxをセットする
		nowState_ = animStates_[idx].get();
		nowState_->Attach();
		return;
	}
	if (idx == nowState_->GetAnimIdx() || nowState_->IsBlend())
	{
		// 同じかブレンド中の時は処理しない
		return;
	}
	// ブレンド処理をする
	blendState_->SetBlendTime(blendTime);
	static_cast<AnimationState&>(*blendState_).Init(nowState_, animStates_[idx].get());
	nowState_ = blendState_.get();
	
}

void Animator::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	nowState_ = nowState_->Update(nowState_, delta);
}

void Animator::Begin(ObjectManager& objectManager)
{
	render_ = objectManager.GetComponent<ModelRender>(ownerId_);
	blendState_ = std::make_unique<BlendState>();
}

void Animator::AddAnimation(int animIdx, bool isLoop)
{
	animStates_.emplace(animIdx,std::make_unique<NormalState>());
	animStates_[animIdx]->SetHandle(*render_->GetHandle());
	animStates_[animIdx]->SetUpAnim(animIdx);
	animStates_[animIdx]->SetLoopFlag(isLoop);
}


void NormalState::Init(AnimationState* start, AnimationState* end)
{
	// 何もしない
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	start->Detach();
}

AnimationState* NormalState::Update(AnimationState* state, float delta)
{
	//moveAnimFrameIndex_ = MV1SearchFrame(handle_, L"Center");
	//MV1SetFrameUserLocalMatrix(handle_, moveAnimFrameIndex_, MGetIdent());
	//prevPosition_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
	playTime_ += delta * 60.0f;
	DebugDrawString(playTime_);
	auto a = MV1GetAttachAnimBlendRate(handle_, attachIdx_);
	if (playTime_ >= totalTime_)
	{
		//MV1SetAttachAnimTime(handle_, attachIdx_, totalTime_);
		//nowPosition_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
		//position_ += nowPosition_ - prevPosition_;
		// 総再生時間まで再生したとき
		if (isLoop_)
		{
			// ループするとき再生時間を0にする
			playTime_ = 0.0f;
			//// 次に『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」と
			//// 『アニメーション再生時間０での「アニメーションで移動しているフレームの座標」』との差分をモデルの座標に加算する
			//MV1SetAttachAnimTime(handle_, attachIdx_, 0.0f);
			//prevPosition_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
			//MV1SetAttachAnimTime(handle_, attachIdx_, playTime_);
			//nowPosition_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
			//position_ += nowPosition_ - prevPosition_;
		}
		else
		{
			// ループ再生しないときは総再生時間を入れとく
			playTime_ = totalTime_;
		}
		playTimeOver_ += delta * 60.0f;
	}
	else
	{
		//// 再生時間をセット
		//MV1SetAttachAnimTime(handle_, attachIdx_, playTime_);
		//// 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
		//// 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分をモデルの座標に加算する
		//nowPosition_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
		//position_ += nowPosition_ - prevPosition_;
	}
	return state;
}



void BlendState::Init(AnimationState* start, AnimationState* end)
{
	position_ = VGet(0.0f, 0.0f, 0.0f);
	start_ = start;
	end_ = end;
	//blendTime_ = 1.0f;
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	end_->Attach();
	end_->ReStart();
	MV1SetAttachAnimBlendRate(start_->GetHandle(), start_->GetAttachIdx());
	MV1SetAttachAnimBlendRate(end_->GetHandle(), end_->GetAttachIdx(), 0.0f);
}

AnimationState* BlendState::Update(AnimationState* state, float delta)
{
	// ブレンドするアニメーションを再生処理する
	start_->Update(nullptr, delta);
	end_->Update(nullptr, delta);

	// 秒数に応じてブレンド率を変える
	playTime_ += delta;
	float d = (playTime_ / blendTime_);
	MV1SetAttachAnimBlendRate(end_->GetHandle(), end_->GetAttachIdx(), d);
	MV1SetAttachAnimBlendRate(start_->GetHandle(), start_->GetAttachIdx(), 1.0f - d);
	DebugDrawString(d);
	if (d >= 1.0f)
	{
		// 1以上の時ブレンド終了なので
		start_->Detach();
		return end_;
	}
	
	return state;
}

void AnimationState::ReStart(void)
{
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	MV1SetAttachAnimTime(handle_, attachIdx_, 0.0f);
}

void AnimationState::Attach(void)
{
	attachIdx_ = MV1AttachAnim(handle_, animIdx_);
}

int AnimationState::GetAttachIdx(void)
{
	return attachIdx_;
}

void AnimationState::Detach()
{
	MV1DetachAnim(handle_, attachIdx_);
}

void AnimationState::SetHandle(int handle)
{
	handle_ = handle;
}

const int AnimationState::GetHandle(void) const
{
	return handle_;
}

void AnimationState::SetUpAnim(int idx)
{
	animIdx_ = idx;
	totalTime_ = MV1GetAnimTotalTime(handle_,animIdx_);
}

int AnimationState::GetAnimIdx(void)
{
	return animIdx_;
}
