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
	// アイテムUI
	itemUI_->Update(delta, objMng);
	UpdateGetUse(delta, objMng);

	// スタミナUI
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
		// 取得アイテム更新
		itemUI_->UpdateItem(delta);
	}
	// 使用されたアイテムのIDを取得しObjManagerに渡す
	objMng->SetUseItem(itemUI_->GetUseItem());
	return true;
}

bool UIMng::UpdateSpMag(const std::shared_ptr<ObjManager> objMng)
{
	if (stanimaF_)
	{
		// ダッシュ可能時はスピード倍率を取得しstaminaUIにセット
		staminaUI_->SetSpMag(objMng->GetPlayer().lock()->GetSpMag());
		if (staminaUI_->GetStamina() <= 0.0f)
		{
			stanimaF_ = false;
			effectF_ = true;
		}
	}
	else
	{
		// スタミナ切れ中は強制的に通常スピードに固定
		objMng->GetPlayer().lock()->SetSpMag(0.4f);
		if (effectF_)
		{
			objMng->AddEffect(EffectName::Down, objMng->GetPlayer().lock()->GetPos());
			effectF_ = false;
		}
		if (staminaUI_->GetStamina() >= (staminaUI_->GetGageMaxCnt() * 10.0f / 2.0f))
		{
			// 一定値回復したらダッシュ可能にする
			objMng->AddEffect(EffectName::Up, objMng->GetPlayer().lock()->GetPos());
			stanimaF_ = true;
		}
	}
	return true;
}
