#include <DxLib.h>
#include "Pad.h"
#include "../InputConfig/InputConfig.h"

#include "../../Debug.h"

Pad::Pad(int padType) :
	nowPadType_{padType}, update_{nullptr}
{
	Init();
}

Pad::~Pad()
{
}

bool Pad::Init(void)
{
	cursorSpeed_ *= 2.0f;
	if (nowPadType_ == DX_PADTYPE_DUAL_SHOCK_4 || nowPadType_ == DX_PADTYPE_DUAL_SENSE)
	{
		// PS�n�̃R���g���[���[�̎�
		update_ = &Pad::UpdatePsPad;
	}
	else if (nowPadType_ == DX_PADTYPE_XBOX_360 || nowPadType_ == DX_PADTYPE_XBOX_ONE)
	{
		// xbox�n�̃R���g���[���[�̎�
		update_ = &Pad::UpdateXboxPad;
	}
	else
	{
		// �Ƃ肠����xbox�n�Ɠ����̓���Ƃ�
		update_ = &Pad::UpdateXboxPad;
	}

	// 0�ŏ��������Ƃ�
	std::fill(std::begin(state_.Buttons), std::end(state_.Buttons), 0);
	return true;
}

void Pad::Update(float delta)
{
	// �p�b�h�̏����擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &state_);
	
	// �{�^���̏����Z�b�g
	for (auto& code : lpConfigMng.GetInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = state_.Buttons[code.second];
	}

	// ���X�e�B�b�N�̏����Z�b�g
	leftInput_ = Vector2{ static_cast<float>(state_.Y), static_cast<float>(-state_.X) };
	if (leftInput_.SqMagnitude() <= Square(500.0f))
	{
		// �X�������l�ȉ��̎��[���ɂ���
		leftInput_ = zeroVector2<float>;
	}
	else
	{
		// �R���g���[���[�̌X���𐳋K�����ăZ�b�g����
		leftInput_.Normalize();
		cursorPos_ += Vector2{ static_cast<float>(-leftInput_.y),static_cast<float>(leftInput_.x),} *cursorSpeed_ * delta;
	}

	// �E�X�e�B�b�N�ƌ���&�L�����Z���{�^���̍X�V
	(this->*update_)(delta);
}

const int Pad::GetPadType(void) const
{
	return nowPadType_;
}

void Pad::SetCursorPos(const Vector2& pos)
{
	cursorPos_ = pos;
	rightInput_ = pos;
}

void Pad::UpdatePsPad(float delta)
{
	// ����{�^���̍X�V
	decisionData_.second = decisionData_.first;
	decisionData_.first = state_.Buttons[1];

	// �L�����Z���{�^���̍X�V
	cancelData_.second = cancelData_.first;
	cancelData_.first = state_.Buttons[9];

	// �E�X�e�B�b�N�̍X�V
	Vector2 move{ static_cast<float>(state_.Z),static_cast<float>(state_.Rz) };
	rightInput_ = zeroVector2<float>;
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	rightInput_ += move * cursorSpeed_ * delta;
}

void Pad::UpdateXboxPad(float delta)
{
	// ����{�^���̍X�V
	decisionData_.second = decisionData_.first;
	decisionData_.first = state_.Buttons[0];
	
	// �L�����Z���{�^���̍X�V
	cancelData_.second = cancelData_.first;
	cancelData_.first = state_.Buttons[7];

	// �E�X�e�B�b�N�̍X�V
	Vector2 move{ static_cast<float>(state_.Rx),static_cast<float>(state_.Ry) };
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	rightInput_ += move * cursorSpeed_ * delta;
}
