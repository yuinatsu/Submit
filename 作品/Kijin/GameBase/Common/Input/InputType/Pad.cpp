#include <DxLib.h>
#include "Pad.h"
#include "../InputConfig/InputConfig.h"

#include "../../Debug.h"

Pad::Pad(int padType) :
	nowPadType_{padType}, update_{nullptr}
{
	if (nowPadType_ == DX_PADTYPE_DUAL_SHOCK_4 || nowPadType_ == DX_PADTYPE_DUAL_SENSE)
	{
		update_ = &Pad::UpdatePsPad;
	}
	else if (nowPadType_ == DX_PADTYPE_XBOX_360 || nowPadType_ == DX_PADTYPE_XBOX_ONE)
	{
		update_ = &Pad::UpdateXboxPad;
	}
	else
	{
		// とりあえずxbox系と同じの入れとく
		update_ = &Pad::UpdateXboxPad;
	}

	std::fill(std::begin(state_.Buttons), std::end(state_.Buttons), 0);
	padState_ = 0;
	Init();
	
}

Pad::~Pad()
{
}

bool Pad::Init(void)
{
	return true;
}

void Pad::Update(float delta)
{
	padState_ = GetJoypadInputState(DX_INPUT_PAD1);
	GetJoypadDirectInputState(DX_INPUT_PAD1, &state_);
	//GetJoypadXInputState(DX_INPUT_PAD1, &xinputState_);

	
	for (auto& code : lpConfigMng.GetInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = state_.Buttons[code.second];
	}

	
	
	
	inputVec_ = Vector2{ static_cast<float>(state_.Y), static_cast<float>(-state_.X) };
	if (inputVec_.SqMagnitude() <= Square(500.0f))
	{
		inputVec_ = zeroVector2<float>;
	}
	else
	{
		inputVec_.Normalize();
	}
	// スティックとかの更新
	(this->*update_)(delta);}

void Pad::SetCursorPos(const Vector2& pos)
{
	cursorPos_ = pos;
}

void Pad::UpdatePsPad(float delta)
{
	Vector2 move{ static_cast<float>(state_.Z),static_cast<float>(state_.Rz) };
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	cursorPos_ += move * cursorSpeed_ * delta;
}

void Pad::UpdateXboxPad(float delta)
{
	//DebugLog("x=", inputVec_.x, "y=", inputVec_.y);
	Vector2 move{ static_cast<float>(state_.Rx),static_cast<float>(state_.Ry) };
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	cursorPos_ += move * cursorSpeed_ * delta;
}
