#include <DxLib.h>
#include <algorithm>
#include "UiBase.h"
#include "../../Input/Pad.h"
#include "../../Input/Keyboard.h"
#include "../../common/nameof.hpp"

UiBase::UiBase(const Math::Vector2& pos, const Math::Vector2& UISize) : pos_(pos), UISize_(UISize)
{
	// コントロール種類
	if (GetJoypadNum())
	{
		// ゲームパッド
		controller_ = std::make_unique<Pad>();
	}
	else
	{
		// キーボード
		controller_ = std::make_unique<Keyboard>();
	}
	// 生存中
	isAlive_ = true;
}

void UiBase::UpdateItem(const double& delta)
{
	// 所持アイテムが最大値だったら抜ける
	if (order_.size() >= haveItemMax)
	{
		return;
	}
	// アイテム名検索
	for (auto& id : id_)
	{
		for (int n = 0; n < id_.size(); n++)
		{
			for (int i = 0; i < static_cast<int>(ItemName::Max); i++)
			{
				if (NAMEOF_ENUM(static_cast<ItemName>(i)) != id.second)
				{
					continue;
				}
				bool isLead = false;
				// 先頭アイテム
				if (!i)
				{
					isLead = true;
				}
				// アイテム格納
				order_.push_back(std::make_pair(i, isLead));
				break;
			}
			break;
		}
	}
	// 受け取ってきたid_をクリア
	id_.clear();
}

bool UiBase::SetItemID(IdData id)
{
	if (order_.size() < haveItemMax)
	{
		// アイテム追加
		id_.push_back(id);
		return true;
	}
	return false;
}

ItemName UiBase::GetUseItem()
{
	// 使用されたアイテムを格納
	auto name = useItem_;
	// アイテム未使用状態に戻す
	useItem_ = ItemName::Max;
	return name;
}
