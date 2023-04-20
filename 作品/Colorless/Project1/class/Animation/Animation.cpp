#include <DxLib.h>
#include "Animation.h"
#include "../common/AnimMng.h"
#include "../common/ImageMng.h"

Animation::Animation(std::string key):key_(key)
{
	Init();
}

Animation::~Animation()
{
}

bool Animation::Init(void)
{
	animCount_ = 0;
	animFrame_ = 0;
	state_ = "up";
	return true;
}

bool Animation::Update(void)
{
	// アニメーションがなかった場合
	if (!lpAnimMng.CheckAnim(key_,state_))
	{
		return false;
	}
	// フレーム異常値
	if (animFrame_ < 0 || animFrame_ >= lpAnimMng.GetAnimSize(key_,state_))
	{
		return false;
	}
	// カウンタ異常値
	if (animCount_++ >= lpAnimMng.GetAnimFrame(key_, state_, animFrame_))
	{
		if (lpAnimMng.GetAnimID(key_, state_, animFrame_) >= 0)
		{
			animFrame_ += 1;
		}
	}
	// フレームが上限に達したらリセット
	if (animFrame_ >= lpAnimMng.GetAnimSize(key_, state_))
	{
		animFrame_ = 0;
		animCount_ = 0;
	}
	return true;
}

bool Animation::Draw(Vector2F pos,Vector2 size,float mag)
{
	// 拡大率計算
	size = size * mag;
	// 描画
	DrawRotaGraph(pos.x + (size.x/static_cast<float>(2)),pos.y + (size.y/static_cast<float>(2)),mag,0.0f, lpAnimMng.GetAnimID(key_, state_, animFrame_),true);
	return true;
}

bool Animation::state(const std::string state)
{
	// アニメーションがなかった場合
	if (!lpAnimMng.CheckAnim(key_, state))
	{
		return false;
	}
	// 状態変化時は各数値リセット
	if (state != state_)
	{
		animFrame_ = 0;
		animCount_ = 0;
	}
	// 格納
	state_ = state;
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
