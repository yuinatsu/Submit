#include <DxLib.h>
#include "UIMng.h"
#include "ItemUi.h"
#include "StaminaUi.h"
#include "../ObjManager.h"

UIMng::UIMng()
{
	itemUI_ = std::make_shared<ItemUi>(Math::Vector2{ 20,550 }, Math::Vector2{ 269,128 });
	staminaUI_ = std::make_shared<StaminaUi>(Math::Vector2{ 300,615 }, Math::Vector2{ 73,1 });
	stanimaF_ = true;
	effectF_ = false;
}

UIMng::~UIMng()
{
}

bool UIMng::Update(float delta, const std::shared_ptr<ObjManager> objMng)
{
	// �A�C�e��UI
	itemUI_->Update(delta, objMng);
	UpdateGetUse(delta, objMng);

	// �X�^�~�iUI
	staminaUI_->Update(delta, objMng);
	UpdateSpMag(objMng);
	return true;
}

void UIMng::Draw(const double& delta)
{
	itemUI_->DrawUI(delta);
	staminaUI_->DrawUI(delta);
}

bool UIMng::FinStart()
{
	return (itemUI_->IsStartFin() && staminaUI_->IsStartFin());
}

bool UIMng::UpdateGetUse(float delta, const std::shared_ptr<ObjManager> objMng)
{
	if (itemUI_->GetItemID().size() > 0)
	{
		// �擾�A�C�e���X�V
		itemUI_->UpdateItem(delta);
	}
	// �g�p���ꂽ�A�C�e����ID���擾��ObjManager�ɓn��
	objMng->SetUseItem(itemUI_->GetUseItem());
	return true;
}

bool UIMng::UpdateSpMag(const std::shared_ptr<ObjManager> objMng)
{
	if (stanimaF_)
	{
		// �_�b�V���\���̓X�s�[�h�{�����擾��staminaUI�ɃZ�b�g
		staminaUI_->SetSpMag(objMng->GetPlayer().lock()->GetSpMag());
		if (staminaUI_->GetStamina() <= 0.0f)
		{
			stanimaF_ = false;
			effectF_ = true;
		}
	}
	else
	{
		// �X�^�~�i�؂ꒆ�͋����I�ɒʏ�X�s�[�h�ɌŒ�
		objMng->GetPlayer().lock()->SetSpMag(0.4f);
		if (effectF_)
		{
			objMng->AddEffect(EffectName::Down, objMng->GetPlayer().lock()->GetPos());
			effectF_ = false;
		}
		if (staminaUI_->GetStamina() >= (staminaUI_->GetGageMaxCnt() * 10.0f / 2.0f))
		{
			// ���l�񕜂�����_�b�V���\�ɂ���
			objMng->AddEffect(EffectName::Up, objMng->GetPlayer().lock()->GetPos());
			stanimaF_ = true;
		}
	}
	return true;
}
