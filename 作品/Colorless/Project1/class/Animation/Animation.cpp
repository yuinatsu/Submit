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
	// �A�j���[�V�������Ȃ������ꍇ
	if (!lpAnimMng.CheckAnim(key_,state_))
	{
		return false;
	}
	// �t���[���ُ�l
	if (animFrame_ < 0 || animFrame_ >= lpAnimMng.GetAnimSize(key_,state_))
	{
		return false;
	}
	// �J�E���^�ُ�l
	if (animCount_++ >= lpAnimMng.GetAnimFrame(key_, state_, animFrame_))
	{
		if (lpAnimMng.GetAnimID(key_, state_, animFrame_) >= 0)
		{
			animFrame_ += 1;
		}
	}
	// �t���[��������ɒB�����烊�Z�b�g
	if (animFrame_ >= lpAnimMng.GetAnimSize(key_, state_))
	{
		animFrame_ = 0;
		animCount_ = 0;
	}
	return true;
}

bool Animation::Draw(Vector2F pos,Vector2 size,float mag)
{
	// �g�嗦�v�Z
	size = size * mag;
	// �`��
	DrawRotaGraph(pos.x + (size.x/static_cast<float>(2)),pos.y + (size.y/static_cast<float>(2)),mag,0.0f, lpAnimMng.GetAnimID(key_, state_, animFrame_),true);
	return true;
}

bool Animation::state(const std::string state)
{
	// �A�j���[�V�������Ȃ������ꍇ
	if (!lpAnimMng.CheckAnim(key_, state))
	{
		return false;
	}
	// ��ԕω����͊e���l���Z�b�g
	if (state != state_)
	{
		animFrame_ = 0;
		animCount_ = 0;
	}
	// �i�[
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
