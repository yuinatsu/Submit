#include <DxLib.h>
#include "StaminaUi.h"
#include "../../common/Debug.h"
#include "../../Scene/SceneMng.h"
#include "../../common/ImageMng.h"

StaminaUi::StaminaUi(const Math::Vector2& pos, const Math::Vector2& gageDiv) : UiBase(pos, gageDiv)
{
	pos_ = pos;
	gageMaxCnt_ = (int)gageDiv.x;
	stamina_ = (float)gageMaxCnt_ * 10.0f;
	isDash_ = false;
	isMove_ = false;
	canDash_ = true;
	updateFunc_ = std::bind(&StaminaUi::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);
	drawFunc_ = std::bind(&StaminaUi::DrawStart, this, std::placeholders::_1);
	count_ = 0.0f;
	startFinSta_ = false;
	size_ = Math::Vector2(447, 30);
	ext_ = 0.0f;
	memCnt_ = 0.0f;

	lpImageMng.GetID("Resource/Image/UI/stamina.png", "Box");
	lpImageMng.GetID("Resource/Image/UI/gage1.png", "Gage", Vector2(3, 18), Vector2(73, 1));
}

StaminaUi::~StaminaUi()
{
}

void StaminaUi::Update(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	updateFunc_((float)delta, objMng);
}

void StaminaUi::UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	controller_->Update(delta);
	count_ += (float)delta;
	// �o�ꎞ���o
	if (count_ < 0.5f && ext_ < 1.0f)
	{
		ext_ += 1.0f / 25.0f;
	}
	// �Q�[���J�n
	else if ((int)memCnt_ < 73 && count_ > 1.0f)
	{
		memCnt_ += (float)delta * 30.0f;
	}
	else if ((int)memCnt_ >= 73)
	{
		startFinSta_ = true;
		updateFunc_ = std::bind(&StaminaUi::UpdateGame, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&StaminaUi::DrawGame, this, std::placeholders::_1);
	}
	if (!controller_->IsAnyPress())
	{
		ext_ = 1.0f;
		memCnt_ = 73;
		startFinSta_ = true;
		updateFunc_ = std::bind(&StaminaUi::UpdateGame, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&StaminaUi::DrawGame, this, std::placeholders::_1);
	}
}

void StaminaUi::UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	controller_->Update(delta);
	isDash_ = controller_->Press(InputID::Dash);
	isMove_ = controller_->MovePress(true);

	UpdateStamina(delta);
}

void StaminaUi::UpdateStamina(const double& delta)
{
	auto dashMove = isDash_ && isMove_;
	// �_�b�V�����쒆���_�b�V���\�Ȃ���s
	if (dashMove && canDash_)
	{
		// �X�^�~�i�����炷
		// ���Ⴊ�ݒ��̓X�^�~�i�����炳�Ȃ�
		if (!controller_->Press(InputID::Crouch))
		{
			SetStaminaValue(-500.0 * delta);
		}
		if (stamina_ <= 0.0)
		{
			// �X�^�~�i�l��0�����������_�b�V���s�ɂ���
			canDash_ = false;
		}
	}
	// �X�^�~�i�����^������Ȃ����_�b�V���s�\�Ȃ���s
	else if (!dashMove && (stamina_ < gageMaxCnt_ * 10 || !canDash_))
	{
		// �X�^�~�i��
		SetStaminaValue(200.0 * delta);
		if (stamina_ >= (gageMaxCnt_ * 10.0f) / 2.0f)
		{
			// �X�^�~�i�����l�񕜂�����_�b�V���\�ɂ���
			canDash_ = true;
		}
	}
}

void StaminaUi::SetStaminaValue(const double& value)
{
	stamina_ = stamina_ + static_cast<float>(value);
}

void StaminaUi::DrawUI(const double& delta)
{
	drawFunc_(delta);
}

void StaminaUi::DrawStart(const double& delta)
{
	DrawRotaGraph3F(pos_.x + (size_.x / 2.0f), pos_.y + (size_.y / 2.0f), size_.x / 2.0f, size_.y / 2.0f, 1.0f, ext_, 0.0f, lpImageMng.GetID("Box")[0], true);
	if (count_ > 1.0f)
	{
		if (memCnt_ >= gageMaxCnt_)
		{
			memCnt_ = (float)gageMaxCnt_;
		}
		for (int i = 0; i < (int)(memCnt_); i++)
		{
			DrawGraphF(pos_.x + 6.0f + 6.0f * i, pos_.y + 5.75f, lpImageMng.GetID("Gage")[i], true);
		}
	}
}

void StaminaUi::DrawGame(const double& delta)
{
	DrawRotaGraph2F(pos_.x + (size_.x / 2.0f), pos_.y + (size_.y / 2.0f), size_.x / 2.0f, size_.y / 2.0f, ext_, 0.0f, lpImageMng.GetID("Box")[0], true);
	for (int i = 0; i <= (stamina_ / 10) - 1; i++)
	{
		DrawGraphF(pos_.x + 6.0f + 6.0f * i, pos_.y + 5.75f, lpImageMng.GetID("Gage")[i], true);
	}
}
