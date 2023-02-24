#include <DxLib.h>
#include "Animation.h"
#include "../common/AnimMng.h"
#include "../common/DrawMng.h"
#include "../common/ImageMng.h"
#include "../common/Debug.h"

Animation::Animation(const std::string& fname, const std::string& key, const std::string& state, Obj& owner) : Component{owner}, key_{key}, loop_{0}, playStepTime_{0.0f}
{
	Assert(lpAnimMng.Load(fname,key),"アニメーションデータのロード失敗");
	SetState(state);
}

Animation::~Animation()
{
}

void Animation::Update(float delta)
{
	// 経過時間を追加
	playStepTime_ += delta;
	if (playStepTime_ >= nowItr_->second)
	{
		++nowItr_;
		playStepTime_ = 0.0f;
		auto& data = lpAnimMng.GetDate(key_, state_);
		if (data.dataList.end() == nowItr_)
		{
			if (data.loop == -1 || data.loop > ++loop_)
			{
				nowItr_ = data.dataList.begin();
			}
			else
			{
				nowItr_ = --data.dataList.end();
			}
		}
	}
	return ;
}

bool Animation::Draw(DrawMng& drawMng,Math::Vector2 pos, bool flag)
{
	if (flag)
	{
		// Idle中ならアニメーションストップ
		drawMng.Draw(pos, 0.0f, lpImageMng.GetID(key_)[lpAnimMng.GetDate(key_, state_).dataList.begin()->first]);
	}
	else
	{
		// それ以外はアニメーションあり
		drawMng.Draw(pos, 0.0f, lpImageMng.GetID(key_)[nowItr_->first]);
	}
	return true;
}

bool Animation::Draw(DrawMng& drawMng, Math::Vector2 pos, bool flag, const std::string& key)
{
	if (flag)
	{
		// Idle中ならアニメーションストップ
		drawMng.Draw(pos, 0.0f, lpImageMng.GetID(key)[lpAnimMng.GetDate(key, state_).dataList.begin()->first]);
	}
	else
	{
		// それ以外はアニメーションあり
		drawMng.Draw(pos, 0.0f, lpImageMng.GetID(key)[nowItr_->first]);
	}
	return true;
}

bool Animation::Draw(DrawMng& drawMng, Math::Vector2 pos, Math::Vector2I size, std::string state)
{
	drawMng.Draw(pos, 0.0f, lpImageMng.GetID(key_)[nowItr_->first]);
	return true;
}

bool Animation::Draw(DrawMng& drawMng, Math::Vector2 pos, float depth, bool flag)
{
	if (flag)
	{
		drawMng.Draw(pos, 0.0f,depth, lpImageMng.GetID(key_)[lpAnimMng.GetDate(key_, state_).dataList.begin()->first]);
	}
	else
	{
		drawMng.Draw(pos, 0.0f, depth,lpImageMng.GetID(key_)[nowItr_->first]);
	}
	return true;
}

std::string Animation::GetKey(void)
{
	return key_;
}

std::string Animation::GetState(void)
{
	return state_;
}


void Animation::SetState(const std::string& state)
{
	// 状態が同じならはじく
	if (state_ == state)
	{
		return;
	}
	state_ = state;
	nowItr_ = lpAnimMng.GetDate(key_, state_).dataList.begin();
	loop_ = 0;
}

void Animation::SetStateSame(const std::string& state)
{
	state_ = state;
	nowItr_ = lpAnimMng.GetDate(key_, state_).dataList.begin();
	loop_ = 0;
}

void Animation::SetKey(const std::string& key)
{
	if (key_ == key)
	{
		return;
	}
	key_ = key;
	nowItr_ = lpAnimMng.GetDate(key_, state_).dataList.begin();
	loop_ = 0;
}

bool Animation::IsEnd(void) const
{
	auto& data = lpAnimMng.GetDate(key_, state_);
	// ループ最大数が-1以外かつループ回数が最大数と同じとき再生終了してる
	return data.loop != -1 && data.loop <= loop_;
}

const int Animation::GetNowIdx(void) const
{
	if (nowItr_ == lpAnimMng.GetDate(key_, state_).dataList.end())
	{
		return -1;
	}
	return nowItr_->first;
}

ComponentID Animation::GetID() const
{
	return ComponentID::Animator;
}
