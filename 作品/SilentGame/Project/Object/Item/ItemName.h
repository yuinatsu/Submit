#pragma once

/// <summary> アイテムの種類 </summary>
enum class ItemName
{
    Knife,          // ナイフ
    Decoy,          // デコイ
    Alcohol,        // お酒
    LaundMine,      // 地雷
    Clock,          // 時計
    Key,            // ドアのキーカード
    Max
};

static ItemName begin(ItemName) { return ItemName::Knife; }
static ItemName end(ItemName) { return ItemName::Max; }
static ItemName operator++(ItemName& state) { return (state = static_cast<ItemName>(std::underlying_type<ItemName>::type(state) + 1)); }
static ItemName operator*(const ItemName& state) { return state; }
