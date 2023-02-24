#include "DxLib.h"
#include "GameCommon.h"
#include "Fader.h"

Fader::FADE_STATE Fader::GetState(void)
{
	return stage_;
}

bool Fader::IsEnd(void)
{
	return isEnd_;
}

void Fader::SetFade(FADE_STATE state)
{
	stage_ = state;
	if (stage_ != FADE_STATE::NONE)
	{
		isEnd_ = false;
	}
}

void Fader::Init(void)
{
	stage_ = FADE_STATE::NONE;
	alpha_ = 0;
	isEnd_ = true;
}

void Fader::Update(void)
{

	if (isEnd_)
	{
		return;
	}

	switch (stage_)
	{
	case FADE_STATE::NONE:
		return;

	case FADE_STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			// フェード終了
			alpha_ = 255;
			isEnd_ = true;

		}
		break;

	case FADE_STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			isEnd_ = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw(void)
{

	switch (stage_)
	{
	case FADE_STATE::NONE:
		return;
	case FADE_STATE::FADE_OUT:
	case FADE_STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}
